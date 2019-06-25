#ifndef __CORE_INSTANCE_H__
#define __CORE_INSTANCE_H__

#include <string>
#include <memory>
#include <async/thread_pool.h>
#include <core/settings.h>

namespace core
{
  namespace server
  {
    class instance
    {
    public:
      instance(int argc, char* argv[]) :
        _settings(server::settings::get_settings(argc, argv)),
        _service(),
        _thread_pool(_settings)
      {
      }

    public:
      std::shared_ptr<server::settings> settings()
      {
        return _settings;
      }

      asio::io_service& service()
      {
        return _service;
      }

      async::thread_pool& thread_pool()
      {
        return _thread_pool;
      }

    private:
      std::shared_ptr<server::settings> _settings;
      asio::io_service _service;
      async::thread_pool _thread_pool;
    };
  }
  namespace client
  {
    class instance
    {
    public:
      instance(int argc, char* argv[]) :
        _settings(client::settings::get_settings(argc, argv))
      {
      }

    public:
      std::shared_ptr<client::settings> settings()
      {
        return _settings;
      }

    private:
      std::shared_ptr<client::settings> _settings;
    };
  }
}

#endif//__CORE_INSTANCE_H__
