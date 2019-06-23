#ifndef __GRAPHICS_EXCEPTION_H__
#define __GRAPHICS_EXCEPTION_H__

#include <exception>
#include <string>

namespace graphics
{
  class exception :
    public std::exception
  {
  public:
    inline exception(const std::string& what)
      :_what(what)
    {}

    inline const char* what() const throw()
    {
      return _what.c_str();
    }

  private:
    std::string _what;

  };
}

#endif//__GRAPHICS_EXCEPTION_H__
