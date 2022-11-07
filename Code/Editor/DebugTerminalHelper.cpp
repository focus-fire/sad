#include "sadpch.h"

#include "DebugTerminalHelper.h"

cap::DebugTerminalHelper::DebugTerminalHelper()
{
	add_command_({ "debug", "Log text to the debug sinks", Debug, NoCompletion });
	add_command_({ "echo", "Echo text to the terminal", Echo, NoCompletion });
	add_command_({ "clear", "Clear the terminal", Clear, NoCompletion });
	add_command_({ "close", "Closes the terminal", Close, NoCompletion });
}

std::vector<std::string> cap::DebugTerminalHelper::NoCompletion(argument_type& arg) { return {}; }

void cap::DebugTerminalHelper::Clear(argument_type& arg)
{
	arg.term.clear();
}

void cap::DebugTerminalHelper::Echo(argument_type& arg)
{
	if (arg.command_line.size() < 2)
		return;

	std::string str = std::move(arg.command_line[1]);
	for (auto it = std::next(arg.command_line.begin(), 2); it != arg.command_line.end(); it++)
	{
		str += " " + std::move(*it);
	}

	ImTerm::message message;
	message.value = std::move(str);
	message.color_beg = message.color_end = 0;
	arg.term.add_message(std::move(message));
}

void cap::DebugTerminalHelper::Debug(argument_type& arg)
{
	if (arg.command_line.size() < 2)
		return;

	std::string message = std::move(arg.command_line[1]);
	core::Log(ELogType::Debug, "[cmd] {}", message);
}

void cap::DebugTerminalHelper::Close(argument_type& arg)
{
	arg.term.set_should_close();
}