#ifndef __GRAPHICS_JPEG_STREAM_H__
#define __GRAPHICS_JPEG_STREAM_H__

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
    class stream_errhandler
    {
    private:
      typedef api::jpeg_error_mgr error_mgr_t;
      typedef api::j_common_ptr common_ptr_t;

    public:
      inline stream_errhandler():
        _error_mgr{ 0 }
      {
      }
     
    protected:
      template<typename c_ptr_t>
      void initialize(c_ptr_t common)
      {
        initialize((common_ptr_t)common);
      }

      void initialize(common_ptr_t common)
      {
        common->client_data = this;
        common->err = api::jpeg_std_error(&_error_mgr);
        _error_mgr.error_exit = error_manager;
      }

    private:
      static api::noreturn_t error_manager(common_ptr_t handle)
      {
        ((stream_errhandler*)handle->client_data)->throw_error();
      }

    protected:
      virtual void throw_error() = 0;

    private:
      error_mgr_t _error_mgr;

    };

    template<typename streamT>
    class stream:
      public stream_errhandler
    {
    public:
      typedef streamT stream_t;
      typedef streamT* handle_t;

    public:
      inline stream() :
        _handle(new stream_t{ 0 })
      {
        stream_errhandler::initialize(handle());
      }

      virtual ~stream()
      {
      }

    public:
      operator handle_t() const
      {
        return handle();
      }

    protected:
      handle_t handle() const
      {
        return _handle.get();
      }

    public:
      size image_size() const
      {
        return size(handle()->image_width, handle()->image_height);
      }

    protected:
      virtual void throw_error() override
      {
        throw exception("stream jpeg exception");
      }

    private:
      std::shared_ptr<stream_t> _handle;

    };

    typedef stream<api::jpeg_decompress_struct> decompress_stream;
    typedef stream<api::jpeg_compress_struct> compress_stream;

  }
}

#endif//__GRAPHICS_JPEG_STREAM_H__
