
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include <graphics/graphics.h>
#include <graphics/jpeg/decoder.h>
#include <jpeglib/jpeglib.h>
#include <setjmp.h>
#include <vector>
#include <iterator>

#pragma warning(disable:4996)

void test_app_text()
{
  using namespace graphics;

  try {
    font def_font(font::types::medium, 0, font::flags::none);
    canvas def_canvas(size(640, -480), graphics::bpp::b24, canvas::flags::none);
    def_canvas.fill(color::white);

    print_text(def_canvas, def_font, "Hello World!", position(10, 10), color::black);

    def_canvas.save("test.bmp");
  }
  catch (const std::exception &e)
  {
    std::cout << "error:" << e.what() << std::endl;
  }
}

void test_decode_jpeg()
{
  std::ifstream file("input.jpg", std::ios::binary);

  graphics::jpeg::buffer_type buffer(std::istreambuf_iterator<char>(file), {});

  graphics::jpeg::decoder decoder;

  decoder.initialize(buffer);
  graphics::canvas image = decoder.decompress();

  image.save("out.bmp");
}

int main(int argc, char* argv[])
{

  test_decode_jpeg();
  return 0;
}