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
		~DebugTerminal();

		void Start();
		void Render();

	private: 
		ImTerm::terminal<DebugTerminalHelper> m_Terminal;
		DebugTerminalCommandItems m_TerminalCommands;

		spdlog::sink_ptr m_TerminalSinkPtr;

		bool m_IsVisible;
	};
}
