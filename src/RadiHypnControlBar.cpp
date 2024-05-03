#include <RadiHypnControlBar.hpp>

RadiHypnControlBar::RadiHypnControlBar()
    : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL), m_button_play("Play"),
      m_button_stop("Stop"), m_button_clipboard("Copy to clipboard") {
  set_valign(Gtk::ALIGN_END);
  pack_start(m_button_play, Gtk::PACK_SHRINK);
  pack_start(m_button_stop, Gtk::PACK_SHRINK);
  pack_start(m_button_clipboard, Gtk::PACK_SHRINK);
}
