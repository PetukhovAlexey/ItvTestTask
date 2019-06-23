#ifndef __GRAPHICS_EZDLIB_CANVAS_H__
#define __GRAPHICS_EZDLIB_CANVAS_H__

#include <graphics/ezdlib/api.h>
#include <graphics/ezdlib/color.h>
#include <graphics/ezdlib/config.h>
#include <graphics/ezdlib/exception.h>
#include <vector>

namespace graphics
{
  namespace ezdlib
  {
    enum class bpp
    {
      b1 = 1,
      b24 = 24,
      b32 = 32
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
    private:
      canvas(const canvas&)
      {}

    public:
      canvas(const size_type& size, bpp bits_per_pixel, flags flags):
        _handle(nullptr)
      {
        _handle = api::ezd_create(size.width(), size.height(), static_cast<int>(bits_per_pixel), static_cast<int>(flags));
      }
      canvas(canvas&& c):
        _handle(c._handle),
        _buffer(std::move(c._buffer))
      {
        c._handle = nullptr;
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

      inline void set_image_buffer(const buffer_type& buff)
      {
        _buffer = buff;
        set_image_buffer();
      }

      inline void set_image_buffer(buffer_type&& buff)
      {
        _buffer.swap(buff);
        buff = buffer_type();
        set_image_buffer();
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
          //api::ezd_destroy(_handle);
          _handle = nullptr;
        }
      }

      inline operator bool() const
      {
        return check();
      }

    private:
      inline void set_image_buffer()
      {
        if (!api::ezd_set_image_buffer(get_handle(), (void*)&_buffer[0], _buffer.size()))
        {
          throw exception("set image buffer canvas exception");
        }
      }

    private:
      handle_t _handle;
      buffer_type _buffer;

    };
  }
}

#endif//__GRAPHICS_EZDLIB_CANVAS_H__
