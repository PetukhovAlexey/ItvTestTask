#ifndef __GRAPHICS_JPEG_ENCODER_H__
#define __GRAPHICS_JPEG_ENCODER_H__

#include <graphics/jpeg/api.h>
#include <graphics/jpeg/config.h>
#include <graphics/jpeg/stream.h>
#include <graphics/jpeg/memory_stream.h>

namespace graphics
{
  namespace jpeg
  {
    class encoder:
      public compress_stream
    {
    public:
      encoder() :
        compress_stream()
      {
        build();
      }

      ~encoder()
      {
        destroy();
      }

    private:
      void build()
      {
        using namespace api;
        jpeg_create_compress(handle());
      }

      void destroy()
      {
        api::jpeg_destroy_compress(handle());
      }

    protected:
      void prepare_stream(const canvas& bt, int quality)
      {
        size_type size = bt.size();
        handle()->image_width = size.width(); 	/* image width and height, in pixels */
        handle()->image_height = size.height();
        handle()->input_components = 3;		/* # of color components per pixel */
        handle()->in_color_space = api::JCS_RGB; 	/* colorspace of input image */

        api::jpeg_set_defaults(handle());
        handle()->do_fancy_downsampling = FALSE;

        api::jpeg_set_quality(handle(), quality, TRUE);
      }

      void start_compress()
      {
        api::jpeg_start_compress(handle(), TRUE);
      }

      void stop_compress()
      {
        api::jpeg_finish_compress(handle());
      }

      void write_line(api::JSAMPARRAY buff)
      {
        (void)api::jpeg_write_scanlines(handle(), buff, 1);
      }

      void static gbr_to_rgb(buffer_type& buff)
      {
        for (size_t i = 0; i < buff.size(); i += 3)
        {
          std::swap(buff[i], buff[i + 2]);
        }
      }

    public:
      buffer_type compress( const canvas& bt, int quality )
      {
        prepare_stream(bt, quality);
        memory_stream res(*this, bt);

        start_compress();
        int row_stride = bt.size().width() * 3;
        api::JSAMPROW row_pointer[1];
        buffer_type line_buffer(count_line_size(handle()->image_width, handle()->num_components));
        while (handle()->next_scanline < handle()->image_height) {
          /* jpeg_write_scanlines expects an array of pointers to scanlines.
           * Here the array is only one element long, but you could pass
           * more than one scanline at a time if that's more convenient.
           */
          bt.get_line(handle()->next_scanline, line_buffer);
          gbr_to_rgb(line_buffer);
          row_pointer[0] = &line_buffer[0];
          write_line( row_pointer);
        }
        stop_compress();

        return res;
      }
    };
  }
}

#endif//__GRAPHICS_JPEG_ENCODER_H__
