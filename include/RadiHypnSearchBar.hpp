#pragma once
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <string>

class RadiHypnSearchBar: public Gtk::Box {
public:
    RadiHypnSearchBar();
    sigc::signal<void(std::string, bool)> signal_search_started();
private:
    void onEntryChanged();

    void onSearchClicked();

    void onClearClicked();

    Gtk::Entry m_entry;
    Gtk::Button m_button_search;
    Gtk::Button m_button_clear;
    Gtk::CheckButton m_checkbox;
    sigc::signal<void(std::string, bool)> m_signal_search_started;

};
