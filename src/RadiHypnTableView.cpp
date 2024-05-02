#include <RadiHypnTableView.hpp>

RadiHypnTableView::RadiHypnTableView() {
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    m_tree_view.set_headers_visible(true);
    m_tree_view.set_hexpand(true);
    m_tree_view.set_vexpand(true);

    m_list_store = Gtk::ListStore::create(m_columns);
    m_tree_view.set_model(m_list_store);
    m_tree_view.set_activate_on_single_click(true);

    Gtk::CellRendererToggle* favourite_toggle = Gtk::manage(new Gtk::CellRendererToggle());
    favourite_toggle->signal_toggled().connect(sigc::mem_fun(*this, &RadiHypnTableView::on_favourite_toggled));

    Gtk::TreeViewColumn* favourite_column = Gtk::manage(new Gtk::TreeViewColumn("Favourite", *favourite_toggle));
    favourite_column->add_attribute(*favourite_toggle, "active", m_columns.m_col_favourite);

    m_tree_view.append_column("Name", m_columns.m_col_name);
    m_tree_view.append_column("URL", m_columns.m_col_url);

    m_tree_view.append_column(*favourite_column);

    m_tree_view.signal_row_activated().connect(sigc::mem_fun(*this, &RadiHypnTableView::on_row_activated));

    m_scrolled_window.add(m_tree_view);
    m_scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    pack_start(m_scrolled_window, true, true, 0);
}

void RadiHypnTableView::populate_table(const std::vector<RadioStream>& streams) {
    m_list_store->clear();

    for (const auto& stream : streams) {
        auto row = *(m_list_store->append());
        row[m_columns.m_col_favourite] = stream.favourite;
        row[m_columns.m_col_name] = stream.name;
        row[m_columns.m_col_url] = stream.url;
    }
}

void RadiHypnTableView::update_table(const std::vector<RadioStream>& streams) {
    std::vector<RadioStream> streams_copy = streams;

    Glib::signal_idle().connect_once([this, streams_copy]() {
        populate_table(streams_copy);
    });
}

void RadiHypnTableView::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
    auto iter = m_list_store->get_iter(path);
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        Glib::ustring name = row[m_columns.m_col_name];
        Glib::ustring url = row[m_columns.m_col_url];
        bool favourite = row[m_columns.m_col_favourite];

        selected.name = name;
        selected.url = url;
        selected.favourite = favourite;

        m_row_activated_signal.emit(path, column);
    }
}

void RadiHypnTableView::on_favourite_toggled(const Glib::ustring& path) {
    auto iter = m_list_store->get_iter(path);
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        row[m_columns.m_col_favourite] = !row[m_columns.m_col_favourite];
        Glib::ustring name = row[m_columns.m_col_name];
        Glib::ustring url = row[m_columns.m_col_url];
        bool favourite = row[m_columns.m_col_favourite];

        m_favourite_toggled_signal.emit(name, url, favourite);
    }
}

RadiHypnTableView::RowActivatedSignal RadiHypnTableView::signal_row_activated() {
    return m_row_activated_signal;
}

RadiHypnTableView::FavouriteToggledSignal RadiHypnTableView::signal_favourite_toggled() {
    return m_favourite_toggled_signal;
}
