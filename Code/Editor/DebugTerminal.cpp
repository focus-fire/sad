#include "sadpch.h"

#include "DebugTerminal.h"

#include <imterm/terminal.hpp>
#include <imterm/utils.hpp>

cap::DebugTerminal::DebugTerminal(DebugTerminalCommandItems commands)
	: m_Terminal(ImTerm::terminal<DebugTerminalHelper>(commands))
	, m_TerminalCommands(commands)
	, m_IsVisible(true)
{
	m_Terminal.theme() = cap::CapTerminalTheme;
}

void cap::DebugTerminal::Start()
{
	DebugTerminalCommandItems commands;
	m_Terminal.set_min_log_level(ImTerm::message::severity::debug);
	
	core::AddLoggingSink(std::move(m_Terminal.get_terminal_helper()));
	core::Log(ELogType::Trace, "Successfully added sink to logging system for editor terminal");
	core::Log(ELogType::Info, "Info log created");
	core::Log(ELogType::Debug, "Debug log created");
}

void cap::DebugTerminal::Render()
{
	if (!m_IsVisible)
		return;
	
	m_IsVisible = m_Terminal.show();
}