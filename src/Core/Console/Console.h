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
		auto Update(float dt) -> bool;
		
		
	private:
		auto HandleInput() -> void;

		std::string m_currentCommand;
		std::vector<std::string> m_history;
		up<ConsoleVisual> m_pConsoleVisual;
		size_t m_cursorPosition = 0;
		bool m_bOnScreen = false;
	};
}