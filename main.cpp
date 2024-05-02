#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include <chrono>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include <utest.h>

#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


#include <SQLiteCpp/SQLiteCpp.h>

#include <gst/gst.h>
#include <atomic>
#include <mutex>
#include <gtkmm.h>
#include <thread>

std::atomic<bool> must_exit(false);
std::mutex searchlock;
std::atomic<int> querylock(0);
std::vector<json> results;
std::string searchquery;
std::atomic<bool> resultsWanted(true);


namespace radihypn {
std::vector<json> searchRadios(std::string term) {
  std::vector<json> r;
  curlpp::Easy request;
  std::string url = "https://de1.api.radio-browser.info/json/stations/search?";
  url += "name=" + curlpp::escape(term);
  url += "&hide_broken=true";
  request.setOpt(new curlpp::options::Url(url));
  request.setOpt(new curlpp::options::FollowLocation(true));

  json parsed;
  std::stringstream buff;
  buff << request;

  buff >> parsed;

  // std::cout << std::setw(4) << parsed;
  for (json::iterator it = parsed.begin(); it != parsed.end(); ++it) {
    r.push_back(*it);
  }

  return r;
}

SQLite::Database getDatabaseConnection() {
  std::string homepath;
  if (const char *env_home = std::getenv("HOME")) {
    homepath = env_home;
  } else {
    throw("$HOME not defined");
  }

  homepath += "/.config/radihypn.sqlite";
  SQLite::Database db(homepath.c_str(), SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

  db.exec("create table IF NOT EXISTS favourites ("
          "station text NOT NULL"
          ")");

  return db;
}

std::vector<json> listFavourites() {
  std::vector<json> r;

  SQLite::Statement query(getDatabaseConnection(),
                          "select station from favourites");

  while (query.executeStep()) {
    const char* station = query.getColumn(0);

    std::stringstream text(station);
    json parsed;
    text >> parsed;
    r.push_back(parsed);
  }

  return r;
}

void addFavourite(json data) {
  std::string text = data.dump();
  SQLite::Statement query(getDatabaseConnection(),
                "INSERT INTO favourites(station) values (?)");
  query.bind(1, text);

  query.exec();
}

void delFavourite(json data) {
  std::string text = data.dump();
  SQLite::Statement query(getDatabaseConnection(),
                "DELETE FROM favourites WHERE station=?");
  query.bind(1, text);

  query.exec();
}

} // namespace radihypn

UTEST(x, testing_works) { ASSERT_TRUE(1); }

// UTEST(x, testing_can_disable_test_by_changing_suite) {
//       ASSERT_TRUE(0);
// }

UTEST(x, search_returns_items) {
  ASSERT_TRUE(radihypn::searchRadios("music").size() > 0);
}

UTEST(x, favourites_works) { radihypn::listFavourites(); }

UTEST(x, favourites_add_delete_works) {
  json k;
  std::stringstream d("{\"a\": 5}");
  d >> k;
  radihypn::addFavourite(k);
  radihypn::delFavourite(k);
}

UTEST(x, structurize_stations) {
  auto stations = radihypn::searchRadios("music");
  for (long unsigned int a = 0; a < stations.size(); a++) {
    auto station = stations[a];
    std::cout << station["name"] << std::endl;
  }
}

void searchThread(Glib::Dispatcher& dispatcher) {
  while (!must_exit) {
    if (querylock > 0) {
      querylock = 0;
      std::string query = searchquery;
      // Perform some time-consuming task
      auto results2 = radihypn::searchRadios(query);
      searchlock.lock();
      results = results2;
      searchlock.unlock();
      dispatcher.emit();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
  }
}


// Global variables
GstElement *pipeline = nullptr;

class SearchApp : public Gtk::Window {
public:
  SearchApp() {
    set_title("Radihypn Radio");
    set_default_size(640, 480);

    // Create search entry
    search_entry.set_placeholder_text("Enter search query");
    search_entry.signal_activate().connect(
        sigc::mem_fun(*this, &SearchApp::on_search_button_clicked));

    // Create search button
    search_button.set_label("Search");
    search_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_search_button_clicked));

    // Create favorites button
    favorites_button.set_label("Favorites");
    favorites_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_favorites_button_clicked));

    // Create result list
    result_list.set_model(result_store);
    result_list.append_column("name", result_columns.name);
    result_list.append_column("url", result_columns.url);

    // Create scrolled window for result list
    result_scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                                      Gtk::POLICY_AUTOMATIC);
    result_scrolled_window.add(result_list);

    //              m_status_icon = Gtk::StatusIcon::create("radio");
    //              m_status_icon->set_visible(true);
    //              m_status_icon->signal_activate().connect(sigc::mem_fun(*this,
    //              &SearchApp::on_m_status_icon_activate));

    // Create play/pause button
    play_pause_button.set_label("Play");
    play_pause_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_play_pause_button_clicked));

    clear_results_button.set_label("Clear");
    clear_results_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_clear_results_button_clicked));

    Gtk::Box *hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
    hbox->pack_start(search_entry);
    hbox->pack_start(search_button, Gtk::PACK_SHRINK);
    hbox->pack_start(favorites_button, Gtk::PACK_SHRINK);
    hbox->pack_start(clear_results_button, Gtk::PACK_SHRINK);

    favor_button.set_label("Favor");
    favor_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_favor_button_clicked));

    clipboard_button.set_label("Copy url");
    clipboard_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SearchApp::on_clipboard_button_clicked));

    Gtk::Box *hbox2 =
        Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
    hbox2->pack_start(play_pause_button, Gtk::PACK_SHRINK);
    hbox2->pack_start(favor_button, Gtk::PACK_SHRINK);
    hbox2->pack_start(clipboard_button, Gtk::PACK_SHRINK);

    // Create box layout
    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
    vbox->pack_start(*hbox, Gtk::PACK_SHRINK);
    vbox->pack_start(result_scrolled_window);
    vbox->pack_start(*hbox2, Gtk::PACK_SHRINK);

    result_list.set_activate_on_single_click(true);
    result_list.signal_row_activated().connect(
        [=](const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column) {
          Gtk::TreeModel::iterator iter = result_store->get_iter(path);
          if (iter) {

            auto row = *iter;
            std::string url = row[result_columns.url];
            // Handle the clicked result here
            // std::cout << "Clicked on result: " << result_text << std::endl;

            if (pipeline != nullptr) {
              gst_element_set_state(pipeline, GST_STATE_NULL);
              gst_object_unref(pipeline);
              pipeline = nullptr;
            }

            pipeline =
                gst_parse_launch(("playbin uri=" + url).c_str(), nullptr);
            gst_element_set_state(pipeline, GST_STATE_PLAYING);

            play_pause_button.set_label("Pause");
          }
        });

    add(*vbox);
    show_all();

    favor_button.set_label("Unfavor");
    // Show favorites or perform favorites-related actions
    // ...

    result_store->clear();

    // Perform search and update result list
    // ...
    auto results = radihypn::listFavourites();

    // Add dummy results to the list for demonstration purposes
    for (long unsigned int i = 0; i < results.size(); i++) {
      // std::string result = std::string("") + results[i]["name"].dump() + " |
      // " + results[i]["url"].dump();
      Gtk::TreeModel::Row row = *(result_store->append());
      row[result_columns.name] = results[i]["name"].dump().substr(0, 45);
      row[result_columns.url] = results[i]["url"].dump();
      row[result_columns.json] = results[i].dump();
    }

  }

public:
  void on_clipboard_button_clicked() {
    Glib::RefPtr<Gtk::Clipboard> clipboard = Gtk::Clipboard::get();
      auto row_iter = (result_list.get_selection()->get_selected());
      if (!row_iter) return;

      auto row = *row_iter;
      std::stringstream data(row[result_columns.json]);
      json j;
      data >> j;
      Glib::ustring clipme = j["url"].dump();
      clipboard->set_text(clipme.substr(1, clipme.size() - 2));
      clipboard->store();
  }

  void on_search_button_clicked() {
    resultsWanted = true;
    // Get search query from the search entry
    std::string search_query = search_entry.get_text();
    if (search_query.length() < 3) {
      search_button.set_label("Search: Enter more than 2 symbols!");
      return;
    }

    search_button.set_label("Search");
    favor_button.set_label("Favor");

    searchquery = search_query;
    querylock++;

    // try {
    //
    //   result_store->clear();
    //
    //   // Perform search and update result list
    //   // ...
    //   auto results = radihypn::searchRadios(search_query);
    //
    //   // Add dummy results to the list for demonstration purposes
    //   for (long unsigned int i = 0; i < results.size(); i++) {
    //     // std::string result = std::string("") + results[i]["name"].dump() + "
    //     // | " + results[i]["url"].dump();
    //     Gtk::TreeModel::Row row = *(result_store->append());
    //     row[result_columns.name] = results[i]["name"].dump().substr(0, 45);
    //     row[result_columns.url] = results[i]["url"].dump();
    //     row[result_columns.json] = results[i].dump();
    //   }
    //
    // } catch (...) {
    // }
  }

  void on_m_status_icon_activate() {
    if (is_visible()) {
      iconify();
      hide();
      // m_status_icon->set_visible(true);
    } else {
      deiconify();
    }
  }

  void on_favorites_button_clicked() {
    resultsWanted = false;
    favor_button.set_label("Unfavor");
    // Show favorites or perform favorites-related actions
    // ...

    result_store->clear();

    // Perform search and update result list
    // ...
    auto results = radihypn::listFavourites();

    // Add dummy results to the list for demonstration purposes
    for (long unsigned int i = 0; i < results.size(); i++) {
      // std::string result = std::string("") + results[i]["name"].dump() + " |
      // " + results[i]["url"].dump();
      Gtk::TreeModel::Row row = *(result_store->append());
      row[result_columns.name] = results[i]["name"].dump().substr(0, 45);
      row[result_columns.url] = results[i]["url"].dump();
      row[result_columns.json] = results[i].dump();
    }
  }

  void on_play_pause_button_clicked() {
    // Toggle play/pause state
    // ...

    auto row_iter = (result_list.get_selection()->get_selected());
    if (!row_iter) return;

    auto row = *row_iter;
    std::string url = row[result_columns.url];
    // std::cout << "Selected: " << url << std::endl;

    if (pipeline) {
      gst_element_set_state(pipeline, GST_STATE_NULL);
      gst_object_unref(pipeline);
      pipeline = nullptr;

      play_pause_button.set_label("Play");
    } else {
      pipeline = gst_parse_launch(("playbin uri=" + url).c_str(), nullptr);
      gst_element_set_state(pipeline, GST_STATE_PLAYING);

      play_pause_button.set_label("Pause");
    }
  }

  void on_favor_button_clicked() {
    if (favor_button.get_label() == "Favor") {

      auto row_iter = (result_list.get_selection()->get_selected());
      if (!row_iter) return;

      auto row = *row_iter;
      std::stringstream data(row[result_columns.json]);
      json j;
      data >> j;
      radihypn::addFavourite(j);

    } else {

      auto row_iter = (result_list.get_selection()->get_selected());
      auto row = *row_iter;
      std::stringstream data(row[result_columns.json]);
      json j;
      data >> j;
      radihypn::delFavourite(j);

      // std::cout << "Selected: " << url << std::endl;
    }
  }

  void on_clear_results_button_clicked() { result_store->clear(); }

public:
  Gtk::Entry search_entry;
  Gtk::Button search_button;
  Gtk::Button clear_results_button;
  Gtk::Button favor_button;
  Gtk::Button favorites_button;
  Gtk::TreeView result_list;
  Gtk::Button play_pause_button;
  Gtk::Button clipboard_button;
  Gtk::ScrolledWindow result_scrolled_window;
  // Glib::RefPtr<Gtk::StatusIcon> m_status_icon;

  class ResultColumns : public Gtk::TreeModel::ColumnRecord {
  public:
    ResultColumns() {
      add(name);
      add(url);
      add(json);
    }

    Gtk::TreeModelColumn<std::string> name;
    Gtk::TreeModelColumn<std::string> url;
    Gtk::TreeModelColumn<std::string> json;
  };

  ResultColumns result_columns;
  Glib::RefPtr<Gtk::ListStore> result_store =
      Gtk::ListStore::create(result_columns);
};

UTEST_STATE();

bool iconified = false;

int main(int argc, char *argv[]) {
  curlpp::Cleanup cleaner;

  if (std::getenv("TESTING")) {
    return utest_main(argc, argv);
  }

  gst_init(&argc, &argv);

  auto app = Gtk::Application::create(argc, argv, "org.searchapp");
  SearchApp search_app;

  Glib::Dispatcher dispatcher;
  dispatcher.connect([&]() {
      if (resultsWanted) {
        search_app.result_store->clear();
        searchlock.lock();
        for (long unsigned int i = 0; i < results.size(); i++) {
          // std::string result = std::string("") + results[i]["name"].dump() + "
          // | " + results[i]["url"].dump();
          Gtk::TreeModel::Row row = *(search_app.result_store->append());
          row[search_app.result_columns.name] = results[i]["name"].dump().substr(0, 45);
          row[search_app.result_columns.url] = results[i]["url"].dump();
          row[search_app.result_columns.json] = results[i].dump();
        }
        results.clear();
        searchlock.unlock();
      }
  });

  Glib::RefPtr<Gtk::StatusIcon> icon = Gtk::StatusIcon::create("radio");
  icon->set_visible(true);
  icon->signal_activate().connect([&search_app]() {
    if (!iconified) {
      search_app.set_skip_taskbar_hint(true);
      search_app.iconify();
    } else {
      search_app.set_skip_taskbar_hint(false);
      search_app.deiconify();
      search_app.present();
    }
    iconified = !iconified;
  });

  querylock = 0;
  std::thread worker_thread(searchThread, std::ref(dispatcher));
  worker_thread.detach();

  return app->run(search_app);

  // Clean up GStreamer
  if (pipeline) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();
  return 0;
}
