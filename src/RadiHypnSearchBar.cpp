#include <RadiHypnSearchBar.hpp>
#include <gtkmm/enums.h>

RadiHypnSearchBar::RadiHypnSearchBar()
    : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL),
    m_entry(),
    m_button_search("Search"),
    m_button_clear("Clear"),
    m_checkbox("Favorites only") {
    set_valign(Gtk::ALIGN_START);

    m_entry.set_placeholder_text("Search...");
    m_entry.set_hexpand(true);
    m_entry.set_activates_default(true);
    m_entry.set_max_length(1);
    pack_start(m_entry, Gtk::PACK_EXPAND_WIDGET);

    m_button_search.set_can_default(true);
    pack_start(m_button_search, Gtk::PACK_SHRINK);

    m_button_clear.set_sensitive(false);
    pack_start(m_button_clear, Gtk::PACK_SHRINK);

    pack_start(m_checkbox, Gtk::PACK_SHRINK);

    m_entry.signal_changed().connect(sigc::mem_fun(*this, &RadiHypnSearchBar::onEntryChanged));
    m_button_search.signal_clicked().connect(sigc::mem_fun(*this, &RadiHypnSearchBar::onSearchClicked));
    m_button_clear.signal_clicked().connect(sigc::mem_fun(*this, &RadiHypnSearchBar::onClearClicked));
    m_entry.signal_activate().connect(sigc::mem_fun(*this, &RadiHypnSearchBar::onSearchClicked));
}

sigc::signal<void(std::string, bool)> RadiHypnSearchBar::signal_search_started() {
    return m_signal_search_started;
}
void RadiHypnSearchBar::onEntryChanged() {
    bool has_text = !m_entry.get_text().empty();
    m_button_clear.set_sensitive(has_text);
}

void RadiHypnSearchBar::onSearchClicked() {
    std::string search_text = m_entry.get_text();
    bool favorites_only = m_checkbox.get_active();
    m_signal_search_started.emit(search_text, favorites_only);
}

void RadiHypnSearchBar::onClearClicked() {
    m_entry.set_text("");
    m_signal_search_started.emit("", m_checkbox.get_active());
}

