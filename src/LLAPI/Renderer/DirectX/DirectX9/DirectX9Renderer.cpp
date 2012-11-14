
// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
//#include <d3dx9.h>

#include <cassert>

#include "Renderer/DirectX/DirectXUtils.h"
#include "DirectX9Renderer.h"
#include "DirectX9DebugTextRenderer.h"
#include "DirectX9Shader.h"

#include "Renderer/IRenderer/RenderWindow.h"
#include "Renderer/IRenderer/RendererUtils.h"
#include "Renderer/IRenderer/VertexBuffer.h"
#include "Renderer/IRenderer/IndexBuffer.h"
#include "Renderer/IRenderer/Texture.h"
#include "Renderer/IRenderer/Color.h"
#include "Math/Matrix.h"

namespace ro3d
{
	namespace Render
	{
		// --------------------------------------------------------------------------------
		DirectX9Renderer::DirectX9Renderer()
		: m_pDevice(nullptr)
		{
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::Initialize()
		{
			HWND hWnd = reinterpret_cast<HWND>(m_pRenderWindow->GetOSHandle());
			SetWindowTextW(hWnd, L"ro3d [DX9]");
			/////
			// create Direct3D device

			LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

			D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

			ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
			d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //disable vsync
			d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D

			// create a device class using this information and information from the d3dpp stuct
			d3d->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&d3dpp,
				&m_pDevice);

			/////
			// set the viewport

			D3DVIEWPORT9 viewport;
			ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));

			viewport.X = 0;
			viewport.Y = 0;
			viewport.Width = m_pRenderWindow->GetWidth();
			viewport.Height = m_pRenderWindow->GetHeight();
			viewport.MinZ = 0.0f;
			viewport.MaxZ = 1.0f;

			m_pDevice->SetViewport(&viewport);

			/////
			// create projection matrices
			m_pPerspectiveProjectionMatrix = new Math::Matrix();
			m_pOrthographicProjectionMatrix = new Math::Matrix();
//			D3DXMatrixPerspectiveFovLH((D3DXMATRIX *)(void *)m_pPerspectiveProjectionMatrix, 3.141592f * 0.25f, 1.25f, 0.01f, 1000.f);
//			D3DXMatrixOrthoLH((D3DXMATRIX *)(void *)m_pOrthographicProjectionMatrix, (float)viewport.Width, (float)viewport.Height, 0.01f, 1000.f);

			InitializeStateObjects();

			m_bInitialized = true;
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::InitializeStateObjects()
		{
			
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::ClearBackground(const Color &color)
		{
			// clear the window to a deep blue
			m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, (uint32_t)color, 0.0f, 0);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::BeginDraw()
		{
			m_pDevice->BeginScene();
		}
		
		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EndDraw()
		{
			m_pDevice->EndScene();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::SwapBuffers()
		{
			m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::Cleanup()
		{
			m_pDevice->Release();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::Update(float dt)
		{

		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::CreateShader(Shader *pShader)
		{
			/*
			DirectX9Shader *pSh = static_cast<DirectX9Shader *>(pShader);
			switch(pSh->GetType())
			{
			case eST_VertexShader:
				{
					LPD3DXBUFFER pShaderByteCode = nullptr;
					LPD3DXBUFFER pErrorMsg = nullptr;
					// compile vertex shader from file
					HR_ERR(D3DXCompileShaderFromFileA(pSh->GetId().GetStrId().c_str(), nullptr, nullptr, "OakVertexShader", "vs_2_0", 0, &pShaderByteCode, &pErrorMsg, nullptr), pErrorMsg);
					
					// create DirectX vertex shader
					if(pShaderByteCode)
					{
						IDirect3DVertexShader9 *pCompiledShader = nullptr;
						m_pDevice->CreateVertexShader((DWORD *)pShaderByteCode->GetBufferPointer(), &pCompiledShader);
						pSh->SetCompiledShader(pCompiledShader);

						// create input layout
						void *pLayoutDesc = nullptr;
						uint32_t numElems;
						CreateInputLayoutDesc(pSh->GetVertexFormat(), pLayoutDesc, numElems);						
						D3DVERTEXELEMENT9 * pld = (D3DVERTEXELEMENT9 *)pLayoutDesc;
						IDirect3DVertexDeclaration9 *pInputLayout = nullptr;
						HR(m_pDevice->CreateVertexDeclaration(pld, &pInputLayout));
						pSh->SetInputLayout(pInputLayout);

						return ;
					}
					break;
				}
			case eST_PixelShader:
				{
					LPD3DXBUFFER pShaderByteCode = nullptr;
					LPD3DXBUFFER pErrorMsg = nullptr;
					// compile pixel shader from file
					HR(D3DXCompileShaderFromFileA(pSh->GetId().GetStrId().c_str(), nullptr, nullptr, "OakPixelShader", "ps_2_0", 0, &pShaderByteCode, &pErrorMsg, nullptr));
					IDirect3DPixelShader9 *pCompiledShader = nullptr;
					// create DirectX pixel shader
					if(pShaderByteCode)
					{
						m_pDevice->CreatePixelShader((DWORD *)pShaderByteCode->GetBufferPointer(), &pCompiledShader);
						pSh->SetCompiledShader(pCompiledShader);

						return;
					}
					break;
				}
			default:
				assert("Shader was not correctly initialized!" && 0);
			}
			pShader->SetCompiledShader(nullptr);
			*/
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::ReleaseShader(Shader *pShader)
		{
			if(pShader == nullptr)
				return;
			((IDirect3DResource9 *)pShader->GetCompiledShader())->Release();
			pShader->SetCompiledShader(nullptr);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::CreateTexture( Texture *pTexture )
		{
//			IDirect3DTexture9 *pTex;
//			const std::string path = pTexture->GetId().GetStrId();
//			D3DXIMAGE_INFO ili;
			
//			HR(D3DXGetImageInfoFromFileA(path.c_str(), &ili));
//			HR(D3DXCreateTextureFromFileA(m_pDevice, path.c_str(), &pTex));
			
			// store created texture in our container
/*			pTexture->SetData(pTex);

			// fill texture properties
			pTexture->SetWidth(ili.Width);
			pTexture->SetHeight(ili.Height);

			switch(ili.Format)
			{
			case D3DFMT_A8R8G8B8:
				pTexture->SetFormat(Texture::eTF_A8R8G8B8_UINT);
				break;
			case D3DFMT_X8R8G8B8:
				pTexture->SetFormat(Texture::eTF_X8R8G8B8_UINT);
				break;
			default:
				pTexture->SetFormat(Texture::eTF_UNKNOWN);
			}
			*/
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::ReleaseTexture( Texture *pTexture )
		{
			((IDirect3DResource9 *)pTexture->GetData())->Release();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UseTexture( Texture *pTexture )
		{
			if(pTexture == nullptr)
				return;
			m_pDevice->SetTexture(0, (IDirect3DTexture9 *)pTexture->GetData());
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::DrawPrimitives(uint32_t numPrimitives, uint32_t startVertex /* = 0 */)
		{
			D3DPRIMITIVETYPE pt = D3DPT_TRIANGLELIST;
			switch( m_currentPrimitiveTopology )
			{
			case ePT_PointList:
				pt = D3DPT_POINTLIST;
				break;
			case ePT_LineList:
				pt = D3DPT_LINELIST;
				break;
			case ePT_LineStrip:
				pt = D3DPT_LINESTRIP;
				break;
			case ePT_TriangleList:
				pt = D3DPT_TRIANGLELIST;
				break;
			case ePT_TriangleStrip:
				pt = D3DPT_TRIANGLESTRIP;
				break;
			default:
				break;
			}
			m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_pViewMatrix);

			m_pDevice->DrawPrimitive(pt, startVertex, numPrimitives);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::DrawIndexedPrimitives(uint32_t numPrimitives, uint32_t numVertices, uint32_t startIndex /* = 0 */, uint32_t startVertex /* = 0 */)
		{
			D3DPRIMITIVETYPE pt = D3DPT_TRIANGLELIST;
			switch( m_currentPrimitiveTopology )
			{
			case ePT_PointList:
				pt = D3DPT_POINTLIST;
				break;
			case ePT_LineList:
				pt = D3DPT_LINELIST;
				break;
			case ePT_LineStrip:
				pt = D3DPT_LINESTRIP;
				break;
			case ePT_TriangleList:
				pt = D3DPT_TRIANGLELIST;
				break;
			case ePT_TriangleStrip:
				pt = D3DPT_TRIANGLESTRIP;
				break;
			default:
				break;
			}
			m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)m_pViewMatrix);
			m_pDevice->DrawIndexedPrimitive(pt, startVertex, 0, numVertices, startIndex, numPrimitives);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::CreateVertexBuffer( VertexBuffer *pVertexBuffer )
		{
			IDirect3DVertexBuffer9 *pVB = NULL;
			unsigned int length = pVertexBuffer->GetVertexCount() * pVertexBuffer->GetVertexSize();
			
			HR(m_pDevice->CreateVertexBuffer(length, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &pVB, nullptr));
			pVertexBuffer->SetData(pVB);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::LockVertexBuffer( VertexBuffer *pVertexBuffer, void **ppBuff, uint32_t offsetToLock, uint32_t sizeToLock, uint32_t flags )
		{	
			// no offset??
			((IDirect3DVertexBuffer9 *)pVertexBuffer->GetData())->Lock(offsetToLock, sizeToLock, ppBuff, D3DLOCK_DISCARD);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UnlockVertexBuffer( VertexBuffer *pVertexBuffer )
		{	
			((IDirect3DVertexBuffer9 *)pVertexBuffer->GetData())->Unlock();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::ReleaseVertexBuffer( VertexBuffer *pVertexBuffer )
		{
			((IDirect3DVertexBuffer9 *)pVertexBuffer->GetData())->Release();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::CreateIndexBuffer( IndexBuffer *pIndexBuffer )
		{
			IDirect3DIndexBuffer9 *pIB = NULL;
			
			m_pDevice->CreateIndexBuffer(pIndexBuffer->GetIndexCount() * pIndexBuffer->GetIndexSize(), D3DUSAGE_DYNAMIC, pIndexBuffer->GetIndexSize() == 2? D3DFMT_INDEX16: D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIB, nullptr);
			pIndexBuffer->SetData(pIB);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::LockIndexBuffer( IndexBuffer *pIndexBuffer, void **ppBuff, uint32_t offsetToLock, uint32_t sizeToLock, uint32_t flags )
		{	
			((IDirect3DIndexBuffer9 *)pIndexBuffer->GetData())->Lock(offsetToLock, sizeToLock, ppBuff, 0);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UnlockIndexBuffer( IndexBuffer *pIndexBuffer )
		{	
			((IDirect3DIndexBuffer9 *)pIndexBuffer->GetData())->Unlock();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::ReleaseIndexBuffer( IndexBuffer *pIndexBuffer )
		{
			((IDirect3DIndexBuffer9 *)pIndexBuffer->GetData())->Release();
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::OutputText( const std::string &text, uint32_t x, uint32_t y)
		{
			m_pDebugTextRenderer->OutputText(text, x, y);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::CreateInputLayoutDesc(uint32_t vertexFormat, void *&pLayoutDesc, uint32_t &numElems)
		{			
			D3DVERTEXELEMENT9 layout[13];
			numElems = 0;
			int offset = 0;
			if(vertexFormat & VertexBuffer::eVF_XYZ)
			{
				layout[numElems].Stream = 0;
				layout[numElems].Offset = offset;
				layout[numElems].Type = D3DDECLTYPE_FLOAT3;
				layout[numElems].Method = D3DDECLMETHOD_DEFAULT;
				layout[numElems].Usage = D3DDECLUSAGE_POSITION;
				layout[numElems].UsageIndex = 0;
				offset += 12;
				++numElems;
			}
			if(vertexFormat & VertexBuffer::eVF_Normal)
			{				
				layout[numElems].Stream = 0;
				layout[numElems].Offset = offset;
				layout[numElems].Type = D3DDECLTYPE_FLOAT3;
				layout[numElems].Method = D3DDECLMETHOD_DEFAULT;
				layout[numElems].Usage = D3DDECLUSAGE_NORMAL;
				layout[numElems].UsageIndex = 0;
				offset += 12;
				++numElems;
			}
			if(vertexFormat & VertexBuffer::eVF_Diffuse)
			{
				layout[numElems].Stream = 0;
				layout[numElems].Offset = offset;
				layout[numElems].Type= D3DDECLTYPE_D3DCOLOR;
				layout[numElems].Method = D3DDECLMETHOD_DEFAULT;
				layout[numElems].Usage = D3DDECLUSAGE_COLOR;
				layout[numElems].UsageIndex = 0;
				offset += 16;
				++numElems;
			}

			if(vertexFormat & VertexBuffer::eVF_Tex0)
			{
				layout[numElems].Stream = 0;
				layout[numElems].Offset = offset;
				layout[numElems].Type = D3DDECLTYPE_FLOAT2;
				layout[numElems].Method = D3DDECLMETHOD_DEFAULT;
				layout[numElems].Usage = D3DDECLUSAGE_TEXCOORD;
				layout[numElems].UsageIndex = 0;
				offset += 8;
				++numElems;
			}

			// D3DDECL_END()
			layout[numElems].Stream = 0xff;
			layout[numElems].Offset = 0;
			layout[numElems].Type = D3DDECLTYPE_UNUSED;
			layout[numElems].Method = 0;
			layout[numElems].Usage = 0;
			layout[numElems].UsageIndex = 0;
			numElems ++;

			pLayoutDesc = new D3DVERTEXELEMENT9[numElems];
			memcpy(pLayoutDesc, layout, numElems * sizeof(D3DVERTEXELEMENT9));			
		}


		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UseVertexBuffer( VertexBuffer *pVertexBuffer )
		{
			IDirect3DVertexBuffer9 *pBuffer = (IDirect3DVertexBuffer9 *)pVertexBuffer->GetData();
			uint32_t stride = pVertexBuffer->GetVertexSize();
			uint32_t offset = 0;
			m_pDevice->SetStreamSource(0, pBuffer, offset, stride);
			m_pCurrentVertexBuffer = pVertexBuffer;
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UseIndexBuffer( IndexBuffer *pIndexBuffer )
		{
			if(pIndexBuffer == nullptr || pIndexBuffer->GetData() == nullptr)
			{
				m_pCurrentIndexBuffer = nullptr;
				return;
			}

			IDirect3DIndexBuffer9 *pBuffer = (IDirect3DIndexBuffer9 *)pIndexBuffer->GetData();
			m_pDevice->SetIndices(pBuffer);
			m_pCurrentIndexBuffer = pIndexBuffer;
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UsePrimitiveTopology( PrimitiveTopology primitiveTopology )
		{
			m_currentPrimitiveTopology = primitiveTopology;
			switch(primitiveTopology)
			{
			case ePT_PointList:
				m_numVerticesPerPrimitive = 1;
				break;
			case ePT_LineList:
				m_numVerticesPerPrimitive = 2;
				break;
			case ePT_LineStrip:
				m_numVerticesPerPrimitive = 2;
				break;
			case ePT_TriangleList:
				m_numVerticesPerPrimitive = 3;
				break;
			case ePT_TriangleStrip:
				m_numVerticesPerPrimitive = 3;
				break;
			default:
				break;
			}
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::UseShader( Shader *pShader )
		{
			
			if(pShader->GetType() == eST_VertexShader)
			{
				IDirect3DVertexShader9 *pVertexShader = (IDirect3DVertexShader9 *) pShader->GetCompiledShader();
				m_pDevice->SetVertexDeclaration( (IDirect3DVertexDeclaration9 *)((DirectX9Shader*)pShader)->GetInputLayout() );
			}
			else
			{
				IDirect3DPixelShader9 *pPixelShader = (IDirect3DPixelShader9 *) pShader->GetCompiledShader();
				m_pDevice->SetPixelShader(pPixelShader);
			}
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EnableDepthBuffer()
		{
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::DisableDepthBuffer()
		{
			m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		}

		// --------------------------------------------------------------------------------
		ro3d::Math::Matrix DirectX9Renderer::CreateViewMatrix(ro3d::Math::Vector3 eye, ro3d::Math::Vector3 lookAt, ro3d::Math::Vector3 up)
		{
			ro3d::Math::Matrix mat;
//			D3DXMatrixLookAtLH((D3DXMATRIX *)&mat, (D3DXVECTOR3 *)&eye, (D3DXVECTOR3 *)&lookAt, (D3DXVECTOR3 *)&up);
			return mat;
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EnableOrtographicProjection()
		{
			m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_pOrthographicProjectionMatrix);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EnablePerspectiveProjection()
		{
			m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)m_pPerspectiveProjectionMatrix);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EnableFillWireframe()
		{
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		// --------------------------------------------------------------------------------
		void DirectX9Renderer::EnableFillSolid()
		{
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}	// namespace Render
}	// namespace ro3d