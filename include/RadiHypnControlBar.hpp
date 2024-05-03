#pragma once
#include <gtkmm.h>
#include <gtkmm/box.h>

class RadiHypnControlBar: public Gtk::Box {
public:
    RadiHypnControlBar();

    Gtk::Button m_button_play;
    Gtk::Button m_button_stop;
    Gtk::Button m_button_clipboard;
};
