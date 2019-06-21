#ifndef __GRAPHICS_CANVAS_H__
#define __GRAPHICS_CANVAS_H__

#include "api.h"
#include "color.h"
#include "config.h"
#include "exception.h"

namespace graphics
{
  namespace ezdlib
  {
    enum class color_format
    {
      monochrome = 1,
      rgb = 24,
      rgba = 32
    };

    class canvas
    {
    public:
      typedef api::HEZDIMAGE handle_t;

    public:
      enum class flags
      {
        none = 0,
        user_image_buffer = EZD_FLAG_USER_IMAGE_BUFFER
      };

    public:
      canvas(const size& size, color_format bits_per_pixel, flags flags):
        _handle(nullptr)
      {
        _handle = api::ezd_create(size.width(), size.height(), static_cast<int>(bits_per_pixel), static_cast<int>(flags));
      }

      virtual ~canvas()
      {
        destroy();
      }

    public:
      inline operator handle_t() const
      {
        return get_handle();
      }

    private:
      inline handle_t get_handle() const
      {
        if (!check())
        {
          throw exception("initialize image error");
        }
        return _handle;
      }

      inline bool check() const
      {
        return _handle != nullptr;
      }

    public:
      inline void fill(color color) const
      {
        if (api::ezd_fill(get_handle(), color) == 0)
        {
          throw exception("fill canvas exception");
        }
      }

      inline void save(const char* file_name) const
      {
        if (api::ezd_save(get_handle(), file_name) == 0)
        {
          throw exception("save image error");
        }
      }

      inline void destroy()
      {
        if (_handle != nullptr)
        {
          api::ezd_destroy(_handle);
          _handle = nullptr;
        }
      }

      inline operator bool() const
      {
        return check();
      }

    private:
      handle_t _handle;

    };
  }
}

#endif//__GRAPHICS_CANVAS_H__
