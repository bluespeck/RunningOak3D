#pragma once

#include <string>
#include <vector>
#include "OakVR/Updatable.h"
#include "Utils/Types.h"

namespace oakvr
{
	class ConsoleVisual;

	class Console : public Updatable
	{
	public:
		bool Update(float dt) override;
		
		
	private:
		void HandleInput();

		std::string m_currentCommand;
		std::vector<std::string> m_history;
		up<ConsoleVisual> m_pConsoleVisual;
		int m_cursorPosition = 0;
		bool m_bOnScreen = false;
	};
}