#ifndef __CORE_SETTINGS_H__
#define __CORE_SETTINGS_H__

#include <string>

namespace core
{
  namespace def
  {
    extern const int server_port;
    extern const int jpeg_quality;
    extern const int max_requests_in_work;
    extern const int socket_buffer_size;
    extern const int maximum_message_size;
    extern const int threads_in_pool;
    extern const int delay_seconds;
    extern const std::string address;
    extern const std::string input_jpeg_file;
    extern const std::string output_jpeg_file;
    extern const std::string text_for_image;
  }

  class settings
  {
  public:
    virtual int server_port() = 0;
    virtual int socket_buffer_size() = 0;
    virtual bool abort() = 0;
  };
  namespace server
  {
    class settings :
      public core::settings
    {
    public:
      virtual int jpeg_quality() = 0;
      virtual int max_requests_in_work() = 0;
      virtual int maximum_message_size() = 0;
      virtual int threads_in_pool() = 0;

    public:
      static std::shared_ptr<settings> get_settings(int argc, char* argv[]);
    };
  }
  namespace client
  {
    class settings :
      public core::settings
    {
    public:
      virtual std::string address() = 0;
      virtual std::string input_jpeg_file() = 0;
      virtual std::string text_for_image() = 0;
      virtual std::string output_jpeg_file() = 0;
      virtual int delay_seconds() = 0;
    public:
      static std::shared_ptr<settings> get_settings(int argc, char* argv[]);
    };

  }
}

#endif//__CORE_SETTINGS_H__
