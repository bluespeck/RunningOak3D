
#ifndef __OAKVR_INCLUDE_LEAF3D_SCREENSIZE2D_H__
#define __OAKVR_INCLUDE_LEAF3D_SCREENSIZE2D_H__

#include <cstdint>

namespace oakvr
{
	namespace Leaf3D
	{
		struct ScreenSize2D
		{
			ScreenSize2D(uint32_t width = 0, uint32_t height = 0) : width(width), height(height) { }
			uint32_t width, height;
		};
	}
}

#endif
