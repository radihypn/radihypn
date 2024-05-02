#pragma once
#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <vector>
#include <RadioStream.hpp>
#include <RadioBrowser.hpp>

struct SearchQuery {
    int id;
    std::string query;
    std::function<void(int, std::vector<RadioStream>)> callback;
};

class SearchThread {
private:
    std::thread mThread;
    std::mutex mMutex;
    std::condition_variable mCondVar;
    std::queue<SearchQuery> mMessageQueue;
    bool mStopRequested;

    RadioBrowserApiEndpoint api;

    void searchThreadFunc();

public:
    SearchThread();
    ~SearchThread();
    void startSearch(SearchQuery q);
};
