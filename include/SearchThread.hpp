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

class SearchThread {
private:
    std::thread mThread;
    std::mutex mMutex;
    std::condition_variable mCondVar;
    std::queue<std::pair<std::string, std::function<void(std::vector<RadioStream>)>>> mMessageQueue;
    bool mStopRequested;

    RadioBrowserApiEndpoint api;

    void searchThreadFunc();

public:
    SearchThread();
    ~SearchThread();
    void startSearch(std::string query, std::function<void(std::vector<RadioStream>)> callback);
};
