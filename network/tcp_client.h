#ifndef __NETWORK_TCP_CLIENT_H__
#define __NETWORK_TCP_CLIENT_H__

#include <network/api.h>
#include <iostream>
#include <core/instance.h>
#include <network/message.h>
#include <async/event.h>

namespace network
{
  namespace client
  {
    using asio::ip::tcp;

    typedef std::deque<network::Request> message_queue;

    class session
    {
    public:
      session(
        core::client::instance& instance,
        asio::io_context& io_context,
        const tcp::resolver::results_type& endpoints)
        :
        instance_(instance),
        io_context_(io_context),
        socket_(io_context),
        buffer_(instance.settings()->socket_buffer_size())
      {
        do_connect(endpoints);
      }

      void write(const network::Request& msg)
      {
        asio::post(io_context_,
          [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
      }

      void close()
      {
        asio::post(io_context_, [this]() { socket_.close(); });
      }

    private:
      void do_connect(const tcp::resolver::results_type& endpoints)
      {
        asio::async_connect(socket_, endpoints,
          [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_body();
          }
          connection_event_.signal();
        });
      }

      void do_read_body()
      {
        asio::async_read(socket_,
          asio::buffer(buffer_),
          asio::transfer_at_least(1),
          [this](std::error_code ec, std::size_t size)
        {
          if (!ec)
          {
            raw_message_.insert(raw_message_.end(), buffer_.begin(), buffer_.begin() + size);
          }
          if (socket_.available() == 0)
          {
            socket_.close();
            get_response_event_.signal();
          }
          else
          {
            do_read_body();
          }
        });
      }

      void do_write()
      {
        next_message_ = write_msgs_.front().SerializeAsString();
        asio::async_write(socket_,
          asio::buffer(next_message_),
          [this](std::error_code ec, std::size_t)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
      }

    public:
      network::Response get_response()
      {
        get_response_event_.wait_signal();
        network::Response res;
        res.ParseFromString(raw_message_);
        return res;
      }

      void wait_connection()
      {
        connection_event_.wait_signal();
      }

    private:
      core::client::instance& instance_;
      asio::io_context& io_context_;
      tcp::socket socket_;
      message_queue write_msgs_;
      std::string raw_message_;
      network::storage_type buffer_;
      std::string next_message_;
      async::event get_response_event_;
      async::event connection_event_;
    };
  }
}

#endif//__NETWORK_TCP_CLIENT_H__
