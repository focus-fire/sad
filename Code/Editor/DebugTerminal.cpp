#include "sadpch.h"

#include "DebugTerminal.h"

#include <imterm/terminal.hpp>
#include <imterm/utils.hpp>

#include <Engine/Application.h>

cap::DebugTerminal::DebugTerminal(DebugTerminalCommandItems commands)
	: m_Terminal(ImTerm::terminal<DebugTerminalHelper>(commands, "sadTerminal"))
	, m_TerminalCommands(commands)
	, m_IsVisible(true)
{
	m_TerminalSinkPtr = m_Terminal.get_terminal_helper();
}

cap::DebugTerminal::~DebugTerminal()
{
	// Remove logging sink once terminal is destroyed
	// Prevents from deadlock that occurs when spdlog sends a log to a dead sink
	core::RemoveLoggingSink(m_TerminalSinkPtr);
}

void cap::DebugTerminal::Start()
{
	m_Terminal.theme() = cap::CapTerminalTheme;
	m_Terminal.set_min_log_level(ImTerm::message::severity::debug);

	core::AddLoggingSink(m_TerminalSinkPtr);
	core::Log(ELogType::Info, "Welcome to the sadEngine!");

	float terminalWindowWidth = sad::Application::s_MainWindow->GetWidth();
	m_Terminal.set_size(ImVec2(terminalWindowWidth, 325.0f));
}

void cap::DebugTerminal::Render()
{
	if (!m_IsVisible)
		return;

	m_IsVisible = m_Terminal.show();
}