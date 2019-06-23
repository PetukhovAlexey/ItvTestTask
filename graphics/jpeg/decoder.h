#ifndef __GRAPHICS_JPEG_DECODER_H__
#define __GRAPHICS_JPEG_DECODER_H__

#include <graphics/jpeg/api.h>
#include <graphics/jpeg/config.h>
#include <graphics/jpeg/exception.h>
#include <string>
#include <memory>
#include <algorithm>
#include <graphics/jpeg/stream.h>

namespace graphics
{
  namespace jpeg
  {
    class decoder:
      public decompress_stream
    {
    public:
      inline decoder() :
        decompress_stream()
      {
        build();
      }

      ~decoder()
      {
        destroy();
      }

    private:
      void build()
      {
        using namespace api;
        jpeg_create_decompress(handle());
      }

    public:
      canvas decompress(const buffer_type& binJpg)
      {
        api::jpeg_mem_src(handle(), (const unsigned char*)&binJpg[0], binJpg.size());
        return decompress();
      }

    private:
      canvas decompress()
      {
        buffer_type res; 
        decompress_image_buffer(res);
        return to_canvas(std::move(res));
      }

      void destroy()
      {
        api::jpeg_destroy_decompress(handle());
      }

    private:
      void start_decompress()
      {
        jpeg_start_decompress(handle());
      }

      void stop_decompress()
      {
        jpeg_finish_decompress(handle());
      }

      void read_line(api::JSAMPARRAY buff)
      {
        (void)jpeg_read_scanlines(handle(), buff, 1);
      }

      canvas to_canvas(buffer_type&& buf)
      {
        size s = image_size();
        canvas res(size(s.width(), -s.height()), bpp::b24, canvas::flags::none);
        res.set_image_buffer(std::move(buf));
        return res;
      }

      void decompress_image_buffer(buffer_type& res)
      {
        api::JSAMPARRAY buff;
        api::jpeg_read_header(handle(), TRUE);
        int row_stride = handle()->image_width * handle()->num_components;
        /* Make a one-row-high sample array that will go away when done with image */
        buff = (*handle()->mem->alloc_sarray)
          ((api::j_common_ptr)handle(), JPOOL_IMAGE, row_stride, 1);
        start_decompress();

        while (handle()->output_scanline < handle()->output_height) {
          read_line(buff);

          
          for (size_t i = 0; i < row_stride; i += 3)
          {
            res.push_back(buff[0][i + 2]);
            res.push_back(buff[0][i + 1]);
            res.push_back(buff[0][i + 0]);
          }
          size_t needle = count_line_size(handle()->image_width, handle()->num_components);
          for (size_t i = row_stride; i < needle; ++i)
          {
            res.push_back(255);
          }
        }

        stop_decompress();
      }

    private:
      void throw_error() override
      {
        throw exception("decompress jpeg exception");
      }

    };

  }
}

#endif//__GRAPHICS_JPEG_DECODER_H__
