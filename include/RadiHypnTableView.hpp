#pragma once

#include <gtkmm.h>
#include <vector>
#include <RadioStream.hpp>

class RadiHypnTableView : public Gtk::Box {
public:
    RadiHypnTableView();
    void populate_table(const std::vector<RadioStream>& streams);
    void update_table(const std::vector<RadioStream>& streams);

    typedef sigc::signal<void(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*)> RowActivatedSignal;
    typedef sigc::signal<void(const Glib::ustring&, const Glib::ustring&, bool)> FavouriteToggledSignal;

    RowActivatedSignal signal_row_activated();
    FavouriteToggledSignal signal_favourite_toggled();

    RadioStream selected = RadioStream {
        .name = "",
        .url = "",
        .favourite = false
    };

private:
    void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void on_favourite_toggled(const Glib::ustring& path);

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns() {
            add(m_col_favourite);
            add(m_col_name);
            add(m_col_url);
        }

        Gtk::TreeModelColumn<bool> m_col_favourite;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
        Gtk::TreeModelColumn<Glib::ustring> m_col_url;
    };

    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::TreeView m_tree_view;
    Gtk::CellRendererToggle* m_favourite_toggle;

    RowActivatedSignal m_row_activated_signal;
    FavouriteToggledSignal m_favourite_toggled_signal;
public:
    Glib::RefPtr<Gtk::ListStore> m_list_store;
    ModelColumns m_columns;
};
