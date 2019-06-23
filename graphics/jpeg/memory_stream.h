#ifndef __GRAPHICS_JPEG_MEMORY_STREAM_H__
#define __GRAPHICS_JPEG_MEMORY_STREAM_H__

#include <graphics/jpeg/api.h>
#include <graphics/jpeg/config.h>
#include <graphics/jpeg/stream.h>

namespace graphics
{
  namespace jpeg
  {
    class memory_stream
    {
    public:
      memory_stream(compress_stream& stream, const canvas& canvas) :
        _ptr(nullptr),
        _size(0)
      {
        _size = canvas.get_image_memory_size();
        _ptr = new unsigned char[_size];
        api::jpeg_mem_dest(stream, &_ptr, &_size);
      }
      ~memory_stream()
      {
        delete[] _ptr;
      }

    public:
      operator buffer_type() const
      {
        return buffer_type(_ptr, _ptr + _size);
      }

    private:
      unsigned char* _ptr;
      unsigned long _size;
    };
  }
}

#endif//__GRAPHICS_JPEG_MEMORY_STREAM_H__
