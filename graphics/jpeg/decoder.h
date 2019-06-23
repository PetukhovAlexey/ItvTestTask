#ifndef __GRAPHICS_JPEG_DECODER_H__
#define __GRAPHICS_JPEG_DECODER_H__

#include <graphics/jpeg/api.h>
#include <graphics/jpeg/config.h>
#include <graphics/jpeg/exception.h>
#include <string>
#include <memory>
#include <algorithm>

namespace graphics
{
  namespace jpeg
  {
    class decoder
    {
    public:
      typedef api::jpeg_decompress_struct stream_t;
      typedef api::j_decompress_ptr handle_t;
      typedef api::jpeg_error_mgr error_mgr_t;
      typedef api::j_common_ptr common_ptr_t;

    public:
      inline decoder() :
        _handle(new stream_t{ 0 }),
        _error_mgr{ 0 }
      {
        build();

      }

    public:
      operator handle_t() const
      {
        return handle();
      }

      void initialize(const buffer_type& binJpg)
      {
        api::jpeg_mem_src(handle(), (const unsigned char*)&binJpg[0], binJpg.size());
      }

    private:
      handle_t handle() const
      {
        return _handle.get();
      }

      void build()
      {
        _handle->client_data = this;
        {
          using namespace api;
          jpeg_create_decompress(_handle.get());
        }

        handle()->err = api::jpeg_std_error(&_error_mgr);
        _error_mgr.error_exit = error_manager;
      }

    public:
      size image_size() const
      {
        return size(handle()->image_width, handle()->image_height);
      }

    public:
      canvas decompress()
      {
        buffer_type res; 
        decompress_image_buffer(res);
        return to_canvas(std::move(res));
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
      static api::noreturn_t error_manager(common_ptr_t handle)
      {
        ((decoder*)handle->client_data)->throw_error();
      }

      void throw_error()
      {
        throw exception("decompress jpeg exception");
      }

    private:
      std::shared_ptr<stream_t> _handle;
      error_mgr_t _error_mgr;

    };

  }
}

#endif//__GRAPHICS_JPEG_DECODER_H__
