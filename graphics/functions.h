#ifndef __GRAPHICS_FUNCTIONS_H__
#define __GRAPHICS_FUNCTIONS_H__

#include <string>
#include "canvas.h"
#include "font.h"
#include "position.h"
#include "color.h"

namespace graphics
{
  namespace ezdlib
  {
    inline void print_text(canvas& canvas, font& font,const std::string& text, const position& pos, const color& color)
    {
      api::ezd_text(canvas, font, text.c_str(), -1, pos.x(), pos.y(), color);
    }
  }
}

#endif//__GRAPHICS_FUNCTIONS_H__
