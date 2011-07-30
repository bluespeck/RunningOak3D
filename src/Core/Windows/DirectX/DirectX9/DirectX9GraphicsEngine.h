
// --------------------------------------------------------------------------------
// Written by      Mihai Tudorache 2011
// --------------------------------------------------------------------------------

#ifndef __OAK3D_INCLUDE_DIRECTX9GRAPHICSENGINE_H__
#define __OAK3D_INCLUDE_DIRECTX9GRAPHICSENGINE_H__

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "../../../GraphicsEngine.h"
#include "../DirectXUtils.h"


namespace Oak3D
{
	namespace Core
	{
		class DirectX9DebugText;
		class Shader;

		class DirectX9GraphicsEngine : public GraphicsEngine
		{
		public:

			// constructors
			DirectX9GraphicsEngine();

			// overrides
			virtual void Initialize();
			virtual void Update( float dt );
			virtual void Render();
			virtual void Cleanup();

			virtual void CreateTexture	( Texture *texture );
			virtual void ReleaseTexture	( Texture *texture );
			
			virtual void CreateVertexBuffer	( VertexBuffer *pVertexBuffer );
			virtual void LockVertexBuffer	( VertexBuffer *pVertexBuffer, void **ppBuff, uint32_t offsetToLock = 0, uint32_t sizeToLock = 0, uint32_t flags = 0 );
			virtual void UnlockVertexBuffer	( VertexBuffer *pVertexBuffer );
			virtual void ReleaseVertexBuffer( VertexBuffer *pVertexBuffer );
			virtual void UseVertexBuffer( VertexBuffer *pVertexBuffer );
			
			virtual void CreateIndexBuffer	( IndexBuffer *ibuff );
			virtual void LockIndexBuffer	( IndexBuffer *pIndexBuffer, void **ppBuff, uint32_t offsetToLock = 0, uint32_t sizeToLock = 0, uint32_t flags = 0 );
			virtual void UnlockIndexBuffer	( IndexBuffer *pIndexBuffer );
			virtual void ReleaseIndexBuffer	( IndexBuffer *pIndexBuffer );
			virtual void UseIndexBuffer( IndexBuffer *pIndexBuffer );

			virtual void UsePrimitiveTopology( PrimitiveTopology primitiveTopology );

			virtual void CreateShader( Shader *pShader );
			virtual void ReleaseShader( Shader *pShader );
			virtual void UseShader( Shader *pShader );			

			virtual void OutputText( const std::wstring &text, uint32_t x, uint32_t y);

			// misc
			void CreateInputLayoutDesc( uint32_t vertexFormat, void *&pLayoutDesc, uint32_t &numElems );
			void InitializeStateObjects();

			void EnableDepthBuffer();
			void DisableDepthBuffer();

			IDirect3DDevice9 *GetDevice() { return m_pDevice; }

		private:
			IDirect3DDevice9 *m_pDevice;                     // Direct3D device interface
			IDirect3DSurface9 *m_pRenderTarget;
			DirectX9DebugText *m_pDebugText;		// object used to draw debug text
		};
	}	// namespace Core
}	// namespace Oak3D

#endif

