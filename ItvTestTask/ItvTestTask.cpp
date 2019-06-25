
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include <graphics/graphics.h>
#include <graphics/jpeg/decoder.h>
#include <graphics/jpeg/encoder.h>
#include <jpeglib/jpeglib.h>
#include <network/network.h>
#include <core/instance.h>
#include <core/settings/server.h>

#include <setjmp.h>
#include <vector>
#include <iterator>

std::string append_text_in_jpeg(core::server::instance& instance, const std::string& image_buffer, const std::string& message)
{
  graphics::decoder decoder;

  graphics::canvas image = decoder.decompress(graphics::buffer_type(image_buffer.begin(), image_buffer.end()));
  graphics::font font(graphics::font::types::medium, 20, graphics::font::flags::none);

  print_text(image, font, message, graphics::position(10, 10), graphics::color::black);

  graphics::encoder encoder;

  graphics::buffer_type encoded = encoder.compress(image, instance.settings()->jpeg_quality());
  return std::string(encoded.begin(), encoded.end());
}

network::Response make_error_response(network::response_statuses status,std::string message)
{
  network::Response r;
  r.set_status(status);
  r.set_message(message);
  r.clear_image();
  return r;
}

network::Response append_text_on_jpg(core::server::instance& instance, network::Request req)
{
  network::Response r;
  try {
    r.set_image(append_text_in_jpeg(instance, req.image(), req.message()));
    r.set_status(network::RS_SUCCESS);
    r.set_message("success");
  }
  catch (const std::exception& e)
  {
    return make_error_response(network::RS_ERROR,e.what());
  }
  catch (...)
  {
    return make_error_response(network::RS_ERROR, "uncnown exception");
  }

  return r;
}

void process_request(core::server::instance& instance, std::shared_ptr<network::server::session> session, network::Request req)
{
  network::Response r;
  try {
    instance.thread_pool().enqueue([&instance, session, req]() {
      session->write_message(append_text_on_jpg(instance, req).SerializeAsString());
    });
  }
  catch (const async::exception& e)
  {
    session->write_message(make_error_response(network::RS_REJECTED, e.what()).SerializeAsString());
  }
  catch (const std::exception& e)
  {
    session->write_message(make_error_response(network::RS_ERROR, e.what()).SerializeAsString());
  }
  catch (...)
  {
    session->write_message(make_error_response(network::RS_ERROR, "uncnown exception").SerializeAsString());
  }
}

network::Response reject(core::server::instance& instance)
{
  network::Response r;
  r.set_status(network::RS_REJECTED);
  r.set_message("rejected");

  return r;
}

void test_server(int argc, char* argv[])
{
  try
  {
    core::server::instance instance(argc, argv);
    if (!instance.settings()->abort())
    {

      network::server::server s(instance, process_request, reject);

      instance.service().run();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

int main(int argc, char* argv[])
{
  try {
    test_server(argc, argv);
  }
  catch (const std::exception &e)
  {
    std::cout << "error:" << e.what() << std::endl;
  }
  return 0;
}