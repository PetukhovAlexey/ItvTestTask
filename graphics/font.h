#ifndef __GRAPHICS_FONT_H__
#define __GRAPHICS_FONT_H__

#include "api.h"
#include "exception.h"

namespace graphics
{
  namespace ezdlib
  {

    class font
    {
    public:
      typedef api::HEZDFONT handle_t;
      typedef unsigned char* type_t;

    public:
      enum class types
      {
        string,
        small,
        medium,
        large
      };

      enum class flags
      {
        none = 0,
        invert = EZD_FONT_FLAG_INVERT
      };

      class type 
      {
      public:
        type(const types& t) :
          _e_type(t),
          _cs_type(nullptr)
        {}

        type(type_t t) :
          _e_type(types::string),
          _cs_type(t)
        {}

        operator type_t() const 
        {
          switch (_e_type)
          {
          case types::small:
            return EZD_FONT_TYPE_SMALL;
          case types::medium:
            return EZD_FONT_TYPE_MEDIUM;
          case types::large:
            return EZD_FONT_TYPE_LARGE;
          }
          return _cs_type;
        }

      private:
        font::types _e_type;
        type_t _cs_type;
      };

    public:
      inline font(const type& type, int size = 0, flags flag = flags::none):
        _handle(nullptr)
      {
        _handle = api::ezd_load_font(type, size, static_cast<int>(flag));
      }

      virtual ~font()
      {
        destroy();
      }


      operator handle_t()
      {
        return get_handle();
      }
    private:
      inline handle_t get_handle() const
      {
        if (!check())
        {
          throw exception("create font exception");
        }
        return _handle;
      }

      inline bool check() const
      {
        return _handle != nullptr;
      }

    public:
      inline void destroy()
      {
        if (_handle != nullptr)
        {
          api::ezd_destroy_font(_handle);
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

#endif//__GRAPHICS_FONT_H__
