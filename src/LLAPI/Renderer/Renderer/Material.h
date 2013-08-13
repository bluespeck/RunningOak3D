#pragma once

#include <string>
#include "Math/Vector3.h"
#include "Color.h"

namespace oakvr
{
	namespace render
	{

		class Material
		{
		public:
			std::string m_name;

			oakvr::render::Color m_vAmbient;
			oakvr::render::Color m_vDiffuse;
			oakvr::render::Color m_vSpecular;

			int m_nShininess;
			float m_fAlpha;

			bool m_bSpecular;

		public:
			Material();
			~Material(void);
		};
	} // namespace Render
} // namespace oakvr