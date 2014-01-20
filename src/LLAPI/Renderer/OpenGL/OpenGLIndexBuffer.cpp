
#include "Common.h"

#include "Renderer/Renderer/IndexBuffer.h"
#include "Renderer/Renderer/Renderer.h"


namespace oakvr
{
	namespace render
	{
		class IndexBuffer::IndexBufferImpl
		{
		public:
			IndexBufferImpl();
			
			GLuint m_ibId;
		};

		// --------------------------------------------------------------------------------
		IndexBuffer::IndexBufferImpl::IndexBufferImpl()
		{
		}

		// --------------------------------------------------------------------------------
		IndexBuffer::IndexBuffer()
		:	m_pImpl	{ new IndexBufferImpl() }
		{
		}

		// --------------------------------------------------------------------------------
		IndexBuffer::~IndexBuffer()
		{
		}

		// --------------------------------------------------------------------------------
		void IndexBuffer::Create(uint32_t indexCount, uint8_t indexStride)
		{
			m_count = indexCount;
			m_stride = indexStride;
			glGenBuffers(1, &m_pImpl->m_ibId);
		}

		// --------------------------------------------------------------------------------
		void IndexBuffer::Lock(void **ppBuff, uint32_t offsetToLock, uint32_t sizeToLock, uint32_t flags)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pImpl->m_ibId);
			*ppBuff = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
		}

		// --------------------------------------------------------------------------------
		void IndexBuffer::Unlock()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pImpl->m_ibId);
			glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER);
		}

		// --------------------------------------------------------------------------------
		void IndexBuffer::Release()
		{
			glDeleteBuffers(1, &m_pImpl->m_ibId);
			m_pImpl->m_ibId = -1;
		}

		void IndexBuffer::Use()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pImpl->m_ibId);
		}

	}	// namespace render
}	// namespace oakvr