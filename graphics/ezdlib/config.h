#ifndef __GRAPHICS_EZDLIB_CONFIG_H__
#define __GRAPHICS_EZDLIB_CONFIG_H__

#include <graphics/position.h>
#include <graphics/size.h>
#include <vector>

namespace graphics
{
  namespace ezdlib
  {
    using size_type = graphics::base::size<int>;
    using position_type = graphics::base::position<int>;
    using buffer_type = std::vector<unsigned char>;
  }
}

#endif//__GRAPHICS_EZDLIB_CONFIG_H__
