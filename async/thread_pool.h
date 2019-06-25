#ifndef __ASYNC_THREAD_POOL_H__
#define __ASYNC_THREAD_POOL_H__

#include <thread_pool/ThreadPool.h>
#include <core/settings.h>
#include <async/exception.h>

namespace async
{
  class thread_pool
  {
  private:
    thread_pool(const thread_pool&)
    {}
    thread_pool(thread_pool&&)
    {}
  public:
    thread_pool(std::shared_ptr<core::server::settings> settings) :
      pool(nullptr),
      _settings(settings)
    {
      pool = new ThreadPool(_settings->threads_in_pool());
    }
    ~thread_pool()
    {
      delete pool;
    }


    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
      ->std::future<typename std::result_of<F(Args...)>::type>;

    void mark_as_finish()
    {
      dec_in_work();
    }

    bool can_add_in_work()
    {
      std::unique_lock<std::mutex> lock(_mutex);
      if (_in_work < _settings->max_requests_in_work() )
      {
        ++_in_work;
        return true;
      }
      return false;
    }

    void dec_in_work()
    {
      std::unique_lock<std::mutex> lock(_mutex);
      --_in_work;
    }

  private:
    ThreadPool *pool;
    int _in_work;
    std::mutex _mutex;
    std::shared_ptr<core::server::settings> _settings;

  };

  template<typename T>
  class seve_result_for_return
  {
  private:
    seve_result_for_return(const T& t) :
      tmp(t)
    { }
  public:

    template<class F, class... Args>
    void call(F&& f, Args&&... args)
    {
      tmp = f(args ...);
    }

    T&& get()
    {
      return std::move(tmp);
    }

  private:
    T tmp;
  };

  template<>
  class seve_result_for_return<void>
  {
  private:
  public:
    template<class F, class... Args>
    void call(F&& f, Args&&... args)
    {
      f(args ...);
    }

    void get()
    {
    }

  };

  // add new work item to the pool
  template<class F, class... Args>
  auto thread_pool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
  {
    using return_type = typename std::result_of<F(Args...)>::type;
    if (can_add_in_work())
    {
      return pool->enqueue([this, f](Args... a) -> return_type
      {
        seve_result_for_return<return_type> r;
        r.call(f, args...);
        return r.get();
      }, args ...);
    }
    throw exception("all threads are busy");
  }

}

#endif//__ASYNC_THREAD_POOL_H__
