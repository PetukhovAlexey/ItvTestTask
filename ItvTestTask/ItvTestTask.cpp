
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <graphics/graphics.h>

#pragma warning(disable:4996)


int main(int argc, char* argv[])
{
  using namespace graphics;

  try {
    font def_font(font::types::medium, 0, font::flags::none);
    canvas def_canvas(size(640, -480), bpp::b32, canvas::flags::none);
    def_canvas.fill(color::white);

    print_text(def_canvas, def_font, "Hello World!", position(10, 10), color::black);

    def_canvas.save("test.bmp");
  }
  catch (const std::exception &e)
  {
    std::cout << "error:" << e.what() << std::endl;
  }

  return 0;
}
