
#include <string>
#include <cassert>

#include <d3d11.h>

#include "Renderer/IRenderer/RendererUtils.h"

#include "DirectX11Shader.h"
#include "DirectX11Renderer.h"

#include "Utils/StringID.h"


namespace ro3d
{
	namespace Render
	{
		// --------------------------------------------------------------------------------
		DirectX11Shader::DirectX11Shader()
		: m_pInputLayout(nullptr)
		{
		}
	}	// namespace Render
}	// namespace ro3d