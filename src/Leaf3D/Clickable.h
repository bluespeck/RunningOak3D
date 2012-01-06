
#ifndef _OAK3D_LEAF3D_CLICKABLE_H_
#define _OAK3D_LEAF3D_CLICKABLE_H_

#include <functional>

#include "MouseEvent.h"

using std::tr1::function;
namespace Oak3D
{
	namespace Leaf3D
	{
		class Clickable
		{
		public:
			Clickable();
			
			void SetOnPressHandler( function<void(Event *)> onPressHandler);
			void SetOnReleaseHandler( function<void()> onReleaseHandler);
			void SetOnHoverHandler( function<void()> onHoverHandler);
			
			
		protected:
			function<void()> m_onPressHandler;
			function<void()> m_onReleaseHandler;
			function<void()> m_onHoverHandler;
			
		};
	}
}

#endif