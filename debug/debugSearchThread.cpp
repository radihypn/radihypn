#include "RadioStream.hpp"
#include <SearchThread.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    SearchThread st;
    st.startSearch(SearchQuery{
        .id = 1,
        .query = "relax",
        .callback = [](int id, std::vector<RadioStream> results)
            {
                for (auto it: results) {
                    std::cout << "name: " << it.name <<
                    "\n"  << "url: " << it.url << "\n\n";
                }
            }});
    return 0;
}
