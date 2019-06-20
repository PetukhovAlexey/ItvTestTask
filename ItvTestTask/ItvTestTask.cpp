
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <graphics/font.h>
#include <graphics/canvas.h>

#pragma warning(disable:4996)


int main(int argc, char* argv[])
{
  using namespace graphics::ezdlib;
  using namespace graphics::ezdlib::api;

  font def_font(font::types::medium, 0, font::flags::none);
  canvas def_canvas(640, -480, 24, 0);
  def_canvas.fill(0x606060);

  // Draw some familiar text
  ezd_text(def_canvas, def_font, "Hello World!", -1, 10, 10, 0xffffff);
  def_canvas.save("test.bmp");

  return 0;
}
