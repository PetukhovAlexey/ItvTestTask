#ifndef __GRAPHICS_SIZE_H__
#define __GRAPHICS_SIZE_H__

namespace graphics
{
  namespace base
  {
    template<typename T>
    class size
    {
    public:
      inline size(const T& width = T(), const T& height = T()) :
        _width(width),
        _height(height)
      {}

      virtual ~size() = default;

    public:
      virtual T width() const
      {
        return _width;
      }

      virtual T height() const
      {
        return _height;
      }

      virtual void width(const T& width)
      {
        _width = width;
      }

      virtual void height(const T& height)
      {
        _height = height;
      }

    private:
      T _width;
      T _height;
    };
  }

  using size = base::size<int>;
}

#endif//__GRAPHICS_SIZE_H__
