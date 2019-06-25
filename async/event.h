#ifndef __ASYNC_EVENT_H__
#define __ASYNC_EVENT_H__
#include <condition_variable>

namespace async
{
  class event
  {
  public:
    void signal()
    {
      con_var_.notify_all();
    }

    void wait_signal()
    {
      std::unique_lock<std::mutex> locker(mutex_);
      con_var_.wait(locker);
    }

  private:
    std::condition_variable con_var_;
    std::mutex mutex_;
  };
}

#endif//__ASYNC_EVENT_H__
