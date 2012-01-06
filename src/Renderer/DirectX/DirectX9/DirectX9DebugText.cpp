
#include "Core/Config/Oak3DConfig.h"

#if OAK3D_RENDERER == OAK3D_RENDERER_DIRECTX_9

#include <string>
#include <fstream>
#include <cassert>
#include <cctype>

#include "Oak3D/Engine.h"

#include "DirectX9DebugText.h"
#include "DirectX9Shader.h"
#include "DirectX9GraphicsEngine.h"
#include "Renderer/DirectX/DirectXUtils.h"

#include "Renderer/IRenderer/Font.h"
#include "Renderer/IRenderer/VertexBuffer.h"
#include "Renderer/IRenderer/Texture.h"

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

namespace Oak3D
{
	namespace Render
	{
		// --------------------------------------------------------------------------------
		DirectX9DebugText::DirectX9DebugText()
		: m_pFont(nullptr)
		{
		}

		// --------------------------------------------------------------------------------
		DirectX9DebugText::~DirectX9DebugText()
		{
			if(m_pFont)
			{
				m_pFont->Release();
				delete m_pFont;
			}
		}

		// --------------------------------------------------------------------------------
		void DirectX9DebugText::Init()
		{
			DirectX9GraphicsEngine *ge = (DirectX9GraphicsEngine *)Engine::GetInstance()->GetGraphicsEngine();
			HR(D3DXCreateFont(ge->GetDevice(),     //D3D Device
				22,               //Font height
				0,                //Font width
				FW_NORMAL,        //Font Weight
				1,                //MipLevels
				false,            //Italic
				DEFAULT_CHARSET,  //CharSet
				OUT_DEFAULT_PRECIS, //OutputPrecision
				ANTIALIASED_QUALITY, //Quality
				DEFAULT_PITCH|FF_DONTCARE,//PitchAndFamily
				L"Arial",          //pFacename,
				&m_pFont));         //ppFont
		}

		// --------------------------------------------------------------------------------
		void DirectX9DebugText::OutputText(const std::string &text, uint32_t x, uint32_t y)
		{
			RECT rc = {x, y};
			
			m_pFont->DrawTextA(NULL,        //pSprite
				text.c_str(),  //pString
				-1,          //Count
				&rc,  //pRect
				DT_LEFT|DT_NOCLIP,//Format,
				0xFFFFFFFF); //Color
		}

	} // namespace Render
} // namespace Oak3D

#endif // OAK3D_DIRECTX_9