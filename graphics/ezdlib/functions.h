#ifndef __GRAPHICS_EZDLIB_FUNCTIONS_H__
#define __GRAPHICS_EZDLIB_FUNCTIONS_H__

#include <string>
#include <graphics/ezdlib/canvas.h>
#include <graphics/ezdlib/font.h>
#include <graphics/ezdlib/color.h>

namespace graphics
{
  namespace ezdlib
  {
    inline void print_text(canvas& canvas, font& font,const std::string& text, const position_type& pos, const color& color)
    {
      api::ezd_text(canvas, font, text.c_str(), -1, pos.x(), pos.y(), color);
    }

    inline int count_line_size(int width, int bytes_for_pixel, int align = 4)
    {
      return EZD_SCANWIDTH(width, bytes_for_pixel * CHAR_BIT, align);
    }
  }
}

#endif//__GRAPHICS_EZDLIB_FUNCTIONS_H__
