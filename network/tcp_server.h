#ifndef __NETWORK_TCP_SERVER_H__
#define __NETWORK_TCP_SERVER_H__

#include <network/api.h>
#include <iostream>
#include <core/settings/server.h>
#include <core/instance.h>
#include <network/exception.h>
#include <network/message.h>
#include <algorithm>

namespace network
{
  namespace server
  {
    using asio::ip::tcp;

    class session;

    using handler_t = std::function<void(core::server::instance& instance, std::shared_ptr<session>, network::Request)>;
    using reject_t = std::function<network::Response(core::server::instance& instance)>;

    class session
      : public std::enable_shared_from_this<session>
    {
    private:
      typedef std::vector<unsigned char> storage_t;

    public:
      session(tcp::socket socket, core::server::instance& instance, handler_t processor, reject_t reject)
        : _socket(std::move(socket)),
        _instance(instance),
        _processor(processor),
        _reject(reject)
      {
      }

      ~session()
      {
      }

      void start()
      {
        storage_t temp(_instance.settings()->socket_buffer_size());
        std::swap(_data, temp);
        do_read();
      }

      void write_message(std::string s)
      {
        _resp_message.swap(s);
        do_write();
      }

    private:
      void do_read()
      {
        auto self(shared_from_this());
        asio::async_read(
          _socket,
          asio::buffer(_data, std::min<size_t>(_data.size(), _instance.settings()->maximum_message_size())),
          asio::transfer_at_least(1),
          [this, self](std::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            if (_req_message.size() + length > _instance.settings()->maximum_message_size())
            {
              throw_too_large_message();
              return;
            }
            _req_message.insert(_req_message.end(), _data.begin(), _data.begin() + length);

            if (_socket.available() == 0) {
              process_message();
            }
            else
            {
              do_read();
            }
          }
        });
      }

      void process_message()
      {
        auto self(shared_from_this());
        Request req;
        req.ParseFromString(_req_message);
        _processor(_instance, shared_from_this(), req);
      }


      void do_write()
      {
        auto self(shared_from_this());
        asio::async_write(_socket, asio::buffer(_resp_message),
          [this, self](std::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            if (!_resp_message.empty())
            {
              do_read();
            }
          }
          _resp_message.clear();
        });
      }

      void throw_too_large_message()
      {
        auto r = _reject(_instance);
        write_message(r.SerializeAsString());
      }

      tcp::socket _socket;
      storage_t _data;
      std::string _req_message;
      std::string _resp_message;
      core::server::instance& _instance;
      handler_t _processor;
      reject_t _reject;
    };

    class server
    {
    public:
      server(
        core::server::instance& instance,
        handler_t processor,
        reject_t reject
      )
        : acceptor_(instance.service(), tcp::endpoint(tcp::v4(), instance.settings()->server_port())),
        instance_(instance),
        processor_(processor),
        reject_(reject)
      {
        do_accept();
      }

    private:
      void do_accept()
      {
        acceptor_.async_accept(
          [this](std::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket), instance_, processor_, reject_)->start();
          }

          do_accept();
        });

      }

      tcp::acceptor acceptor_;
      core::server::instance& instance_;
      handler_t processor_;
      reject_t reject_;
    };
  }
}

#endif//__NETWORK_TCP_SERVER_H__
