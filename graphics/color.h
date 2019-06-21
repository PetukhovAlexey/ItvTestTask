#ifndef __GRAPHICS_COLOR_H__
#define __GRAPHICS_COLOR_H__

namespace graphics
{
  namespace ezdlib
  {
    class color
    {
    protected:
      color(const int& c) :
        _color(c)
      {}

    public:
      virtual ~color() = default;

    public:
      operator int() const
      {
        return _color;
      }

      color& operator= (const int& c)
      {
        _color = c;
      }

    public:
      static const color gray;
      static const color white;
      static const color black;

    private:
      int _color;
    };
  }
}

#endif//__GRAPHICS_COLOR_H__
