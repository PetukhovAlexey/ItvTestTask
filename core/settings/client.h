#ifndef __CORE_SETTINGS_CLIENT_H__
#define __CORE_SETTINGS_CLIENT_H__

#include <core/settings.h>

namespace core
{
  namespace client
  {
    namespace def
    {
      class settings :
        public client::settings
      {
      public:
        settings() :
          _server_port(core::def::server_port),
          _address(core::def::address),
          _socket_buffer_size(core::def::socket_buffer_size),
          _input_jpeg_file(core::def::input_jpeg_file),
          _text_for_image(core::def::text_for_image),
          _output_jpeg_file(core::def::output_jpeg_file),
          _delay_seconds(core::def::delay_seconds),
          _abort(false)
        {}

      public:
        int server_port() override
        {
          return _server_port;
        }

        int socket_buffer_size()
        {
          return _socket_buffer_size;
        }

        std::string address() override
        {
          return _address;
        }

        std::string input_jpeg_file() override
        {
          return _input_jpeg_file;
        }

        std::string text_for_image() override
        {
          return _text_for_image;
        }

        std::string output_jpeg_file() override
        {
          return _output_jpeg_file;
        }

        virtual int delay_seconds() override
        {
          return _delay_seconds;
        }

        virtual bool abort() override
        {
          return _abort;
        }

      public:
        int set_server_port(int server_port)
        {
          return _server_port = server_port;
        }
        std::string set_address(std::string address)
        {
          return _address = address;
        }

        int socket_buffer_size(int socket_buffer_size)
        {
          return _socket_buffer_size = socket_buffer_size;
        }

        std::string input_jpeg_file(const std::string& input_jpeg_file)
        {
          return _input_jpeg_file = input_jpeg_file;
        }

        std::string text_for_image(const std::string& text_for_image)
        {
          return _text_for_image = text_for_image;
        }

        std::string output_jpeg_file(const std::string& output_jpeg_file)
        {
          return _output_jpeg_file;
        }

        int delay_seconds(int delay_seconds)
        {
          return _delay_seconds = delay_seconds;
        }

        bool abort(bool abort)
        {
          return _abort = abort;
        }

      private:
        int _server_port;
        int _socket_buffer_size;
        std::string _address;
        std::string _input_jpeg_file;
        std::string _text_for_image;
        std::string _output_jpeg_file;
        int _delay_seconds;
        bool _abort;

      };
    }
}

std::shared_ptr<core::client::settings> core::client::settings::get_settings(int argc, char* argv[])
{
  std::shared_ptr<def::settings> settings(new def::settings());

  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
    {
      std::cout << "(-h|--help) print this message" << std::endl;
      std::cout << "-d \"value\" delays between resending a message in seconds" << std::endl;
      std::cout << "-i \"value\" the path of input jpg image" << std::endl;
      std::cout << "-o \"value\" the path of output jpg image" << std::endl;
      std::cout << "-m \"value\" the message overlay on the image" << std::endl;
      settings->abort(true);
    }
    else if (std::string(argv[i]) == "-d" && (i + 1 < argc))
    {
      settings->delay_seconds(atoi(argv[i + 1]));
      ++i;
    }
    else if (std::string(argv[i]) == "-i" && (i + 1 < argc))
    {
      settings->input_jpeg_file(argv[i + 1]);
    }
    else if (std::string(argv[i]) == "-o" && (i + 1 < argc))
    {
      settings->output_jpeg_file(argv[i + 1]);
    }
    else if (std::string(argv[i]) == "-m" && (i + 1 < argc))
    {
      settings->text_for_image(argv[i + 1]);
    }
  }
  return settings;
}
}

#endif//__CORE_SETTINGS_CLIENT_H__
