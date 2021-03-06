
#ifndef __OAKVR_INCLUDE_LEAF3D_BUTTON_H__
#define __OAKVR_INCLUDE_LEAF3D_BUTTON_H__

#include <string>
#include <functional>

#include "Widget.h"
#include "IListenForMouseEvents.h"

namespace oakvr
{
	namespace Leaf3D
	{
		class Button : public Widget
		{
		public:
			Button();

			virtual void OnMouseLeftButtonHeld(MouseEvent *mev);

		private:
			std::string m_text;	// TODO ?

		};
	}
}

#endif
