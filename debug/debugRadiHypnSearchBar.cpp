#include <gtkmm.h>
#include <RadiHypnSearchBar.hpp>
#include <iostream>

class MyWindow : public Gtk::Window {
public:
    MyWindow()
    : Gtk::Window(),
    m_search_bar() {
        set_title("Search Example");
        set_default_size(400, 200);

        m_search_bar.signal_search_started().connect(sigc::mem_fun(*this, &MyWindow::onSearchStarted));

        add(m_search_bar);
        show_all();
    }

private:
    void onSearchStarted(const std::string& search_text, bool favorites_only) {
        std::cout << "Search started with query: " << search_text << std::endl;
        std::cout << "Favorites only: " << (favorites_only ? "true" : "false") << std::endl;
    }

    RadiHypnSearchBar m_search_bar;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.example.search");

    MyWindow window;

    return app->run(window);
}
