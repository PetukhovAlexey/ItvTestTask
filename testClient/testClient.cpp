// testClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <network/tcp_client.h>
#include <network/message.h>
#include <core/settings/client.h>
#include <core/instance.h>
#include <fstream>

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "asio.hpp"

using network::exception;

 
void load_image(std::string file_name, network::Request& req)
{
  std::ifstream file(file_name, std::ios::binary);
  if (!file)
  {
    throw exception("the input file is corrupted");
  }
  req.set_image(std::string(std::istreambuf_iterator<char>(file), {}));
}

void save_image(network::Response& resp, std::string file_name)
{
  auto image = resp.image();
  std::ofstream out_file(file_name, std::ios::binary);
  std::copy(image.begin(), image.end(), std::ostreambuf_iterator<char>(out_file));
}

network::Request make_request(core::client::instance& instance)
{
  network::Request msg;
  msg.set_message(instance.settings()->text_for_image());
  load_image(instance.settings()->input_jpeg_file(), msg);
  return msg;
}

network::Response send_request(core::client::instance& instance, const network::Request& request)
{
  network::Response resp;
  asio::io_service service;
  asio::ip::tcp::resolver resolver(service);
  auto endpoints = resolver.resolve(instance.settings()->address(), std::to_string(instance.settings()->server_port()));
  network::client::session c(instance, service, endpoints);

  std::thread t([&service]() {
    service.run(); });

  c.wait_connection();
  c.write(request);
  resp = c.get_response();

  t.join();
  return resp;
}

void append_text_on_message(core::client::instance& instance, const network::Request& request)
{
  bool need_repeat;
  do {
    need_repeat = false;
    network::Response response = send_request(instance, request);

    std::cout << "status:" << response.status() << std::endl;
    std::cout << "message:" << response.message() << std::endl;

    switch (response.status())
    {
    case network::RS_REJECTED:
      need_repeat = true;
      std::this_thread::sleep_for(std::chrono::seconds(instance.settings()->delay_seconds()));
      break;
    case network::RS_SUCCESS:
      save_image(response, instance.settings()->output_jpeg_file());
      break;
    case network::RS_ERROR:
      break;
    }

  } while (need_repeat);
}

int main(int argc, char* argv[])
{
  try
  {
    core::client::instance instance(argc, argv);

    if (!instance.settings()->abort())
    {

      append_text_on_message(instance, make_request(instance));
    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
