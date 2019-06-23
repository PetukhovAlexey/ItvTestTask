#ifndef __GRAPHICS_JPEG_FUNCTIONS_H__
#define __GRAPHICS_JPEG_FUNCTIONS_H__

#include <graphics/jpeg/api.h>
#include <graphics/jpeg/decoder.h>
#include <graphics/canvas.h>

namespace graphics
{
  namespace jpeg
  {
    bool check_format(const decoder& instream)
    {
      return api::first_marker(instream);
    }

    canvas decompress(decoder& instream)
    {
      canvas res();
    }
  }
}

#endif//__GRAPHICS_JPEG_FUNCTIONS_H__
