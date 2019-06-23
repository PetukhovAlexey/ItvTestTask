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
    int count_line_size(int width, int bytes_for_pixel, int align = 4);
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

      inline void get_line(int line_count, buffer_type& buffer) const
      {
        size_type sz= size();
        const unsigned char * img_ptr = (const unsigned char *)api::ezd_get_image_ptr(get_handle());
        int line_size = count_line_size(sz.width(), 3);
        const unsigned char * img_ptr_start = line_size * line_count + img_ptr;
        const unsigned char * img_ptr_finish = line_size * (line_count + 1) + img_ptr;

        std::copy(img_ptr_start, img_ptr_finish, buffer.begin());
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

      unsigned long get_image_memory_size() const
      {
        return api::ezd_get_image_size(get_handle()) + api::ezd_header_size();
      }

      inline operator bool() const
      {
        return check();
      }

      size_type size() const
      {
        return size_type(api::ezd_get_width(_handle), std::abs(api::ezd_get_height(_handle)));
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
