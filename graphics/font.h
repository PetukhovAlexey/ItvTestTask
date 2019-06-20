#ifndef __GRAPHICS_FONT_H__
#define __GRAPHICS_FONT_H__

#include "api.h"

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
        _handle(nullptr),
        _type(type),
        _size(size),
        _flag(flag)
      {
      }

      virtual ~font()
      {
        close();
      }


      operator handle_t()
      {
        return open();
      }

    private:
      inline handle_t open()
      {
        if (_handle == nullptr)
        {
          _handle = api::ezd_load_font(_type, _size, static_cast<int>(_flag));
        }
        return _handle;
      }

      inline void close()
      {
        if (_handle != nullptr)
        {
          api::ezd_destroy_font(_handle);
          _handle = nullptr;
        }
      }

    private:
      handle_t _handle;
      type _type;
      int _size;
      flags _flag;
    };
  }
}

#endif//__GRAPHICS_FONT_H__
