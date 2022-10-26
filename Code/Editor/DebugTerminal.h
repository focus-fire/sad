#pragma once

#include "DebugTerminalHelper.h"

namespace cap
{
	/**
	 * @brief ImTerm terminal used to debug the engine, game, and editor
	*/
	class DebugTerminal 
	{
	public:
		explicit DebugTerminal(DebugTerminalCommandItems commands = DebugTerminalCommandItems());

		void Start();
		void Render();

	private: 
		ImTerm::terminal<DebugTerminalHelper> m_Terminal;
		DebugTerminalCommandItems m_TerminalCommands;
		bool m_IsVisible;
	};
}
