#ifndef __GRAPHICS_CONFIG_H__
#define __GRAPHICS_CONFIG_H__

#include "position.h"
#include "size.h"

namespace graphics
{
  namespace ezdlib
  {
    using size = graphics::base::size<int>;
    using position = graphics::base::position<int>;
  }
}

#endif//__GRAPHICS_CONFIG_H__
