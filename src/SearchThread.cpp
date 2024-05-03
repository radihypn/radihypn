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

void SearchThread::startSearch(SearchQuery q) {
  std::unique_lock<std::mutex> lock(mMutex);
  mMessageQueue.push(q);
  mCondVar.notify_one();
}

void SearchThread::searchThreadFunc() {
  while (true) {
    SearchQuery message;

    {
      std::unique_lock<std::mutex> lock(mMutex);
      mCondVar.wait(
          lock, [this] { return !mMessageQueue.empty() || mStopRequested; });

      if (mStopRequested && mMessageQueue.empty()) {
        break;
      }

      message = mMessageQueue.back();
      while (!mMessageQueue.empty()) {
        mMessageQueue.pop();
      }
    }

    std::vector<RadioStream> streams;
    bool ok = false;

    for (int i = 0; i < 10; i++) {
      try {
        streams = api.search(message.query);
        ok = true;
      } catch (...) {
        api.setNextEndpoint();
      }

      if (ok) {
        break;
      }
    }

    if (!ok) {
      throw "network search failed!?!";
    }

    message.callback(message.id, streams);
  }
}
