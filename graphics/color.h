#ifndef __GRAPHICS_COLOR_H__
#define __GRAPHICS_COLOR_H__

namespace graphics
{
  template <class BaseT>
  class color
  {
  protected:
    color(const BaseT& c):
      _color(c)
    {}

  public:
    virtual ~color() = default;

  public:
    operator BaseT() const
    {
      return _color;
    }

    color& operator= (const BaseT& c)
    {
      _color = c;
    }

  private:
    BaseT _color;
  };
}

#endif//__GRAPHICS_COLOR_H__
