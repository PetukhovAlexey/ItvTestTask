#ifndef __CORE_SETTINGS_SERVER_H__
#define __CORE_SETTINGS_SERVER_H__

#include <core/settings.h>

namespace core
{
  namespace server
  {
    namespace def
    {
      class settings:
        public server::settings
      {
      public:
        settings():
          _server_port(core::def::server_port),
          _socket_buffer_size(core::def::socket_buffer_size),
          _jpeg_quality(core::def::jpeg_quality),
          _max_requests_in_work(core::def::max_requests_in_work),
          _maximum_message_size(core::def::maximum_message_size),
          _threads_in_pool(core::def::threads_in_pool),
          _abort(false)
        {}

      public:
        int server_port() override
        {
          return _server_port;
        }

        int socket_buffer_size() override
        {
          return _socket_buffer_size;
        }

        virtual bool abort() override
        {
          return _abort;
        }

        int jpeg_quality() override
        {
          return _server_port;
        }

        int max_requests_in_work() override
        {
          return _max_requests_in_work;
        }

        int maximum_message_size() override
        {
          return _maximum_message_size;
        }
        
        int threads_in_pool() override
        {
          return _threads_in_pool;
        }

      public:
        int set_server_port(int server_port)
        {
          return _server_port = server_port;
        }

        int socket_buffer_size(int socket_buffer_size)
        {
          return _socket_buffer_size = socket_buffer_size;
        }

        int set_jpeg_quality(int jpeg_quality)
        {
          return _server_port = jpeg_quality;
        }

        int set_max_requests_in_work(int max_requests_in_work)
        {
          return _max_requests_in_work = max_requests_in_work;
        }

        int set_maximum_message_size(int maximum_message_size)
        {
          return _maximum_message_size = maximum_message_size;
        }

        int threads_in_pool(int threads_in_pool)
        {
          return _threads_in_pool = threads_in_pool;
        }

        bool abort(bool abort)
        {
          return _abort = abort;
        }

      private:
        int _server_port;
        int _socket_buffer_size;
        int _jpeg_quality;
        int _max_requests_in_work;
        int _maximum_message_size;
        int _threads_in_pool;
        bool _abort;

      };
    }

  }
}

std::shared_ptr<core::server::settings> core::server::settings::get_settings(int argc, char* argv[])
{
  std::shared_ptr<def::settings> settings(new def::settings());

  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
    {
      std::cout << "(-h|--help) print this message" << std::endl;
      std::cout << "-t \"value\" maximum number of concurrent requests" << std::endl;
      settings->abort(true);
    }
    else if (std::string(argv[i]) == "-t" && (i + 1 < argc))
    {
      settings->set_max_requests_in_work(atoi(argv[i + 1]));
      ++i;
    }
  }
  return settings;
}

#endif//__CORE_SETTINGS_SERVER_H__
