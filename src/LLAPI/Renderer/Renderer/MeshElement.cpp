#include "MeshElement.h"

namespace oakvr
{
	namespace render
	{
		MeshElement::MeshElement(const VertexElementDescriptor &vertexFormat, const oakvr::core::MemoryBuffer &vb
			, uint8_t indexStride, const oakvr::core::MemoryBuffer ib
			, std::shared_ptr<Material> &pMaterial)
			: m_vertexData{ vb }, m_indexStride{ indexStride }, m_indexData{ ib }, m_pMaterial{pMaterial}
		{
			m_vertexFormat.push_back(vertexFormat);
			m_vertexStride = vertexFormat.size;
			
		}

		MeshElement::MeshElement(const std::vector<VertexElementDescriptor> &vertexFormat, const oakvr::core::MemoryBuffer &vb
			, uint8_t indexStride, const oakvr::core::MemoryBuffer ib
			, std::shared_ptr<Material> &pMaterial)
			: m_vertexFormat{ vertexFormat }, m_vertexData{ vb }, m_indexStride{ indexStride }, m_indexData{ ib }, m_pMaterial{ pMaterial }
		{
			m_vertexStride = 0;
			for (auto &e : m_vertexFormat)
			{
				m_vertexStride += e.size;
			}
		}

		MeshElement::~MeshElement()
		{

		}

	}
}