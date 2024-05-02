#include <gtkmm.h>
#include "RadiHypnTableView.hpp"
#include <iostream>

class MyWindow : public Gtk::Window {
public:
    MyWindow();

private:
    void on_button_clicked();
    void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void on_favourite_toggled(const Glib::ustring& name, const Glib::ustring& url, bool favourite);

    RadiHypnTableView m_table_view;
    Gtk::Button m_button;
};

MyWindow::MyWindow()
: m_button("Update Table") {
    set_title("RadiHypnTableView Example");
    set_default_size(400, 300);

    m_button.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_button_clicked));

    m_table_view.signal_row_activated().connect(sigc::mem_fun(*this, &MyWindow::on_row_activated));
    m_table_view.signal_favourite_toggled().connect(sigc::mem_fun(*this, &MyWindow::on_favourite_toggled));

    Gtk::Box* vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 5));
    vbox->pack_start(m_table_view, Gtk::PACK_EXPAND_WIDGET);
    vbox->pack_start(m_button, Gtk::PACK_SHRINK);

    add(*vbox);
    show_all();
}

void MyWindow::on_button_clicked() {
    std::vector<RadioStream> streams = {
        {"Stream 1", "http://example.com/stream1", true},
        {"Stream 2", "http://example.com/stream2", false},
        {"Stream 3", "http://example.com/stream3", true},
    };

    m_table_view.update_table(streams);
}

void MyWindow::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
    auto model = m_table_view.m_list_store;
    auto iter = model->get_iter(path);
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        Glib::ustring name = row[m_table_view.m_columns.m_col_name];
        Glib::ustring url = row[m_table_view.m_columns.m_col_url];
        std::cout << "Row activated: " << name << " (" << url << ")" << std::endl;
    }
}

void MyWindow::on_favourite_toggled(const Glib::ustring& name, const Glib::ustring& url, bool favourite) {
    std::cout << "Favourite toggled: " << name << " (" << url << ") - " << (favourite ? "Favourite" : "Not Favourite") << std::endl;
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.example.radiohypn");

    MyWindow window;

    return app->run(window);
}
