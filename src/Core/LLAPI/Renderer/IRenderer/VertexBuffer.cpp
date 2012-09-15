
#include "Oak3D/Engine.h"
#include "VertexBuffer.h"
#include "IRenderer.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Renderer/IRenderer/Color.h"

namespace Oak3D
{
	namespace Render
	{
		// --------------------------------------------------------------------------------
		VertexBuffer::VertexBuffer()
		: m_vertexCount(0)
		, m_pData(nullptr)
		, m_vertexFormat(0)
		, m_vertexSize(0)
		{
		}

		// --------------------------------------------------------------------------------
		VertexBuffer::~VertexBuffer()
		{
			if(m_pData)
				Release();
			m_pData = 0;
		}

		// --------------------------------------------------------------------------------
		void VertexBuffer::Create(uint32_t vertexCount, uint32_t vertexFormat)
		{
			m_vertexCount = vertexCount;
			m_vertexFormat = vertexFormat;
			m_vertexSize = ComputeVertexSizeFromFormat(m_vertexFormat);
			Engine::GetRenderer()->CreateVertexBuffer(this);
		}

		// --------------------------------------------------------------------------------
		uint32_t VertexBuffer::ComputeVertexSizeFromFormat(uint32_t vertexFormat)
		{
			uint32_t vertexSize = 0;
			if(vertexFormat & VertexBuffer::eVF_XYZ)
			{
				vertexSize += sizeof(Oak3D::Math::Vector3);
			}
			if(vertexFormat & VertexBuffer::eVF_Normal)
			{
				vertexSize += sizeof(Oak3D::Math::Vector3);
			}
			if(vertexFormat & VertexBuffer::eVF_Diffuse)
			{
				vertexSize += sizeof(Oak3D::Render::Color);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex0)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex1)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex2)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex3)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex4)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex5)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex6)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}
			if(vertexFormat & VertexBuffer::eVF_Tex7)
			{
				vertexSize += sizeof(Oak3D::Math::Vector2);
			}

			return vertexSize;
		}

		// --------------------------------------------------------------------------------
		void VertexBuffer::Lock(void **ppBuff, uint32_t offsetToLock, uint32_t sizeToLock, uint32_t flags)
		{
			Engine::GetRenderer()->LockVertexBuffer(this, ppBuff, offsetToLock, sizeToLock, flags);
		}

		// --------------------------------------------------------------------------------
		void VertexBuffer::Unlock()
		{
			Engine::GetRenderer()->UnlockVertexBuffer(this);
		}

		// --------------------------------------------------------------------------------
		void VertexBuffer::Release()
		{
			Engine::GetRenderer()->ReleaseVertexBuffer(this);
			m_pData = nullptr;
		}
	}	// namespace Render
}	// namespace Oak3D