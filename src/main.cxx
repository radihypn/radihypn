#include "RadioStream.hpp"
#include <RadiHypnControlBar.hpp>
#include <RadiHypnSearchBar.hpp>
#include <RadiHypnTableView.hpp>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/enums.h>

#include <Database.hpp>
#include <Player.hpp>
#include <SearchThread.hpp>

#include <gtkmm/window.h>
#include <iostream>

#include <xapp/libxapp/xapp-gtk-window.h>
#include <xapp/libxapp/xapp-status-icon.h>

class MyWindow : public Gtk::Window {
public:
  MyWindow() : Gtk::Window(Gtk::WINDOW_TOPLEVEL), m_search_bar() {
    set_title("radihypn");
    set_default_size(400, 500);

    statusIcon = xapp_status_icon_new();
    xapp_status_icon_set_icon_name(statusIcon, "headphones-symbolic");
    xapp_status_icon_set_tooltip_text(statusIcon, "radihypn");

    g_signal_connect_swapped(
        statusIcon, "activate",
        G_CALLBACK(+[](MyWindow *self) { self->onTrayActivate(); }), this);

    m_search_bar.signal_search_started().connect(
        sigc::mem_fun(*this, &MyWindow::onSearchStarted));

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 5));

    vbox->pack_start(m_search_bar, Gtk::PACK_SHRINK);

    m_table_view.signal_favourite_toggled().connect(
        sigc::mem_fun(*this, &MyWindow::onFavouriteToggled));

    vbox->pack_start(m_table_view, Gtk::PACK_EXPAND_WIDGET);

    vbox->pack_end(m_control_bar, Gtk::PACK_SHRINK);

    m_control_bar.m_button_play.signal_clicked().connect(
        sigc::mem_fun(*this, &MyWindow::onPlay));

    m_control_bar.m_button_stop.signal_clicked().connect(
        sigc::mem_fun(*this, &MyWindow::onStop));

    m_control_bar.m_button_clipboard.signal_clicked().connect(
        sigc::mem_fun(*this, &MyWindow::onClipboardClicked));

    add(*vbox);
    show_all();

    onSearchStarted("", true);
  }

  void myhide() {
    iconify();
    set_visible(false);
  }
  void myshow() {
    deiconify();
    set_visible(true);
  }

private:
  void onTrayActivate() {
    if (shown) {
      myhide();
    } else {
      myshow();
    }
    shown = !shown;
  }

  bool shown = true;

  void onPlay() { player.play(m_table_view.selected.url); }
  void onStop() { player.reset(); }
  void onClipboardClicked() {
    Glib::RefPtr<Gtk::Clipboard> clipboard = Gtk::Clipboard::get();
    clipboard->set_text(m_table_view.selected.url);
  }

  void onFavouriteToggled(std::string name, std::string url, bool fav) {
    db.remove(RadioStream{.name = name, .url = url, .favourite = fav});

    if (fav) {
      db.insert(RadioStream{.name = name, .url = url, .favourite = fav});
    }
  }
  void onSearchStarted(const std::string &search_text, bool favorites_only) {
    if (search_text.empty()) {
      m_table_view.update_table(db.listFavourites());
    } else {
      search_id++;
      st.startSearch(SearchQuery{
          .id = search_id,
          .query = search_text,
          .callback = [this, favorites_only](int id,
                                             std::vector<RadioStream> s) {
            if (id != search_id) {
              return;
            }
            Glib::signal_idle().connect_once([this, s, favorites_only]() {
              if (favorites_only) {
                m_table_view.update_table(db.filterFavourites(s));
              } else {
                m_table_view.update_table(db.markFavourites(s));
              }
            });
          }});
    }
  }

  RadiHypnSearchBar m_search_bar;
  RadiHypnTableView m_table_view;
  RadiHypnControlBar m_control_bar;

  Database db;
  SearchThread st;
  Player player;
  int search_id = 0;

  XAppStatusIcon *statusIcon;
};

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create(argc, argv, "com.github.radihypn");
  app->hold();

  MyWindow window;

  window.signal_delete_event().connect([&app] (GdkEventAny* event) {
    app->quit();
    return true;
  });

  return app->run(window);
}
