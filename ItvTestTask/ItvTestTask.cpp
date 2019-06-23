
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
#include <setjmp.h>
#include <vector>
#include <iterator>

#pragma warning(disable:4996)



void do_work(graphics::canvas& def_canvas, graphics::font& def_font)
{
  def_canvas.fill(graphics::color::white);
  print_text(def_canvas, def_font, "Hello World!", graphics::position(10, 10), graphics::color::black);
  def_canvas.save("out.test_app_text.bmp");
}

void test_app_text()
{
  using namespace graphics;

  font def_font(font::types::medium, 0, font::flags::none);
  canvas def_canvas(size(640, -480), graphics::bpp::b24, canvas::flags::none);
  do_work(def_canvas, def_font);
}

void test_decode_jpeg()
{
  std::ifstream file("input.jpg", std::ios::binary);

  graphics::decoder decoder;

  graphics::canvas image = decoder.decompress(graphics::buffer_type(std::istreambuf_iterator<char>(file), {}));

  image.save("out.test_decode_jpeg.bmp");
}

void test_encode_jpeg()
{
  using namespace graphics;

  font def_font(font::types::medium, 20, font::flags::none);
  canvas def_canvas(size(227, -149), graphics::bpp::b24, canvas::flags::none);
  def_canvas.fill(color::white);

  print_text(def_canvas, def_font, "Hello World!", position(10, 10), color::black);

  def_canvas.save("out.test_encode_jpeg.bmp");

  graphics::encoder encoder;

  graphics::buffer_type encoded = encoder.compress(def_canvas, 65);
  std::ofstream out_file("out.test_encode_jpeg.jpg", std::ios::binary);
  std::copy(encoded.begin(), encoded.end(), std::ostreambuf_iterator<char>(out_file));
}

void test_app_text_in_jpeg()
{
  std::ifstream file("input.jpg", std::ios::binary);

  graphics::decoder decoder;

  graphics::canvas image = decoder.decompress(graphics::buffer_type(std::istreambuf_iterator<char>(file), {}));
  graphics::font font(graphics::font::types::medium, 20, graphics::font::flags::none);

  print_text(image, font, "Hello World!", graphics::position(10, 10), graphics::color::black);

  graphics::encoder encoder;

  graphics::buffer_type encoded = encoder.compress(image, 65);
  std::ofstream out_file("out.test_app_text_in_jpeg.jpg", std::ios::binary);
  std::copy(encoded.begin(), encoded.end(), std::ostreambuf_iterator<char>(out_file));
}

int main(int argc, char* argv[])
{
  try {
    test_app_text();
    test_decode_jpeg();
    test_encode_jpeg();
    test_app_text_in_jpeg();
  }
  catch (const std::exception &e)
  {
    std::cout << "error:" << e.what() << std::endl;
  }
  return 0;
}