
#include "RenderWindow.h"

namespace Oak3D
{
	namespace Render
	{
		// --------------------------------------------------------------------------------
		RenderWindow::RenderWindow()
		{
			m_windowState = eWS_Normal;
			m_bFullScreen = false;
			m_osHandle = 0;
			m_posX = m_posY = 0;
			m_width = 1280;
			m_height = 960;
			m_title = L"Oak3D";
		}

		// --------------------------------------------------------------------------------
		RenderWindow::RenderWindow( const std::wstring &title, int posX, int posY, unsigned int width, unsigned int height)
			: m_title(title), m_posX(posX), m_posY(posY), m_width(width), m_height(height)
		{
		}

	}	// namespace Render
}	// namespace Oak3D