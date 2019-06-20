#ifndef __GRAPHICS_POSITION_H__
#define __GRAPHICS_POSITION_H__

namespace graphics
{
  template<typename T>
  class position
  {
  public:
    virtual ~position() = default;

  public:
    virtual T x() const 
    {
      return _x;
    }

    virtual T y() const
    {
      return _y;
    }

    virtual void x(const T& x)
    {
      _x = x;
    }

    virtual void y(const T& y)
    {
      _y = y;
    }

  private:
    T _x;
    T _y;
  };
} 

#endif//__GRAPHICS_POSITION_H__
