#include <RadioStream.hpp>
#include <SearchThread.hpp>

SearchThread::SearchThread() : mStopRequested(false) {
    mThread = std::thread(&SearchThread::searchThreadFunc, this);
}

SearchThread::~SearchThread() {
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mStopRequested = true;
        mCondVar.notify_one();
    }
    if (mThread.joinable()) {
        mThread.join();
    }
}

void SearchThread::startSearch(std::string query, std::function<void(std::vector<RadioStream>)> cb) {
    std::unique_lock<std::mutex> lock(mMutex);
    mMessageQueue.push(std::make_pair(query, cb));
    mCondVar.notify_one();
}

void SearchThread::searchThreadFunc() {
    while (true) {
        std::pair<std::string, std::function<void(std::vector<RadioStream>)>> message;

        {
            std::unique_lock<std::mutex> lock(mMutex);
            mCondVar.wait(lock, [this] { return !mMessageQueue.empty() || mStopRequested; });

            if (mStopRequested && mMessageQueue.empty()) {
                break;
            }

            message = mMessageQueue.front();
            mMessageQueue.pop();
        }

        std::vector<RadioStream> streams;
        bool ok = false;

        for (int i = 0; i < 10; i++) {
            try {
                streams = api.search(message.first);
                ok = true;
            } catch (...) {

            }

            if (ok) {
                break;
            }
        }

        if (!ok) {
            throw "network search failed!?!";
        }

        if (message.second) {
            message.second(streams);
        }
    }
}

