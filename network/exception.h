#ifndef __NETWORK_EXCEPTION_H__
#define __NETWORK_EXCEPTION_H__

#include <exception>
#include <string>

namespace network
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

  class exception_too_large_message:
    public exception
  {
  public:
    exception_too_large_message() :
      exception("buffer overflow exception")
    {}
  };
}

#endif//__NETWORK_EXCEPTION_H__
