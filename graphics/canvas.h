#ifndef __GRAPHICS_CANVAS_H__
#define __GRAPHICS_CANVAS_H__

#include "api.h"

namespace graphics
{
  namespace ezdlib
  {
    class canvas
    {
    public:
      typedef api::HEZDIMAGE handle_t;

    public:
      canvas(int width, int height, int bits_per_pixel, int flags):
        _width(width),
        _height(height),
        _bits_per_pixel(bits_per_pixel),
        _flags(flags)
      {}
      virtual ~canvas()
      {
        close();
      }

    public:
      inline operator handle_t()
      {
        return open();
      }

    public:
      inline handle_t open()
      {
        if (_handle == nullptr)
        {
          _handle = api::ezd_create(_width, _height, _bits_per_pixel, _flags);
        }
        return _handle;
      }

      inline void fill(int color)
      {
        api::ezd_fill(open(), color);
      }

      inline void save(const char* file_name)
      {
        api::ezd_save(open(), file_name);
      }

      inline void close()
      {
        if (_handle != nullptr)
        {
          api::ezd_destroy(_handle);
          _handle = nullptr;
        }
      }

    private:
      handle_t _handle;
      int _width;
      int _height;
      int _bits_per_pixel;
      int _flags;

    };
  }
}

#endif//__GRAPHICS_CANVAS_H__
