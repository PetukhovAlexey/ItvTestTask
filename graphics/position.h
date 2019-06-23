#ifndef __GRAPHICS_POSITION_H__
#define __GRAPHICS_POSITION_H__

namespace graphics
{
  namespace base
  {
    template<typename T>
    class position
    {
    public:
      inline position(const T& x = T(), const T& y = T()) :
        _x(x),
        _y(y)
      {}

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

  using position = base::position<int>;
} 

#endif//__GRAPHICS_POSITION_H__
