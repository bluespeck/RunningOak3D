
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------

#ifndef __OAK3D_INCLUDE_DEBUGTEXT_H__
#define __OAK3D_INCLUDE_DEBUGTEXT_H__

#include <string>
#include <cstdint>

namespace Oak3D
{
	namespace Render
	{
		class DebugText
		{
		public:
			virtual ~DebugText(){}
			virtual void Init() = 0;
			virtual void OutputText(const std::string &text, uint32_t x, uint32_t y) = 0;
		};
	}	// namespace Render
}	// namespace Oak3D

#endif