#ifndef __GRAPHICS_JPEG_API_H__
#define __GRAPHICS_JPEG_API_H__

namespace graphics
{
  namespace jpeg
  {
    namespace api
    {
#include <jpeglib/jpeglib.h>
      extern "C" boolean first_marker(j_decompress_ptr cinfo);

    }
  }
}

#endif//__GRAPHICS_JPEG_API_H__
