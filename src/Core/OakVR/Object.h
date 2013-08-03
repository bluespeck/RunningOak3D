#pragma once

#include <list>

#include "Utils/StringId.h"

namespace oakvr
{
	namespace Render
	{

		class IController;

		class Object
		{
		public:
			Object( void ); 	
			virtual ~Object( void );

			void AddChild(Object *pObj);
			void AddBrother(Object *pObj);

			Object *GetRootObj();
			
		public:
			// links to other objects
			Object *m_pParent;
			Object *m_pChild;
			Object *m_pBrother;

			oakvr::StringId m_objID;

			// Controllers running on this object
			std::list<IController *> m_lstControllers;

		protected:
			static unsigned long m_objCount;

		};
	} // namespace Render
} // namespace oakvr