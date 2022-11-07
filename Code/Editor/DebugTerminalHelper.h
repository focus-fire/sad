#pragma once

#include <vector>
#include <string>

#include <imterm/utils.hpp>
#include <imterm/terminal_helpers.hpp>

namespace cap
{
	/**
	 * @brief Declares variables that can be used to set state for commands in the terminal
	*/
	struct DebugTerminalCommandItems
	{ };

	/**
	 * @brief Defines custom commands and spdlog sink for the DebugTerminal
	*/
	class DebugTerminalHelper : public ImTerm::basic_spdlog_terminal_helper<DebugTerminalHelper, DebugTerminalCommandItems, misc::no_mutex>
	{
	public:
		explicit DebugTerminalHelper();

		/**
		 * @brief Specifies action taken by terminal for autocompleting user results
		*/
		static std::vector<std::string> NoCompletion(argument_type& arg);

		/**
		 * @brief Clears the terminal output
		*/
		static void Clear(argument_type& arg);

		/**
		 * @brief Echoes the entered text into the terminal
		*/
		static void Echo(argument_type& arg);

		/**
		 * @brief Prints to the terminal through the debug logging sinks 
		*/
		static void Debug(argument_type& arg);

		/**
		 * @brief Closes the currently focused terminal
		*/
		static void Close(argument_type& arg);
	};

	/**
	 * @brief Theme with defaults to populate spdlog/ImTerm log level colors into the default ImGui theme
	*/
	constexpr ImTerm::theme CapTerminalTheme
	{
		"Cap",
		std::nullopt,
		std::nullopt,
		std::nullopt,
		std::nullopt,
		std::nullopt,
		std::nullopt,
		std::nullopt, 
		std::nullopt,
		std::nullopt,
		std::nullopt,
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		std::nullopt, 
		{
			ImTerm::theme::constexpr_color{0.549f, 0.561f, 0.569f, 1.f}, // log_level::trace
			ImTerm::theme::constexpr_color{0.153f, 0.596f, 0.498f, 1.f}, // log_level::debug
			ImTerm::theme::constexpr_color{0.459f, 0.686f, 0.129f, 1.f}, // log_level::info
			ImTerm::theme::constexpr_color{0.839f, 0.749f, 0.333f, 1.f}, // log_level::warning
			ImTerm::theme::constexpr_color{1.000f, 0.420f, 0.408f, 1.f}, // log_level::error
			ImTerm::theme::constexpr_color{1.000f, 0.420f, 0.408f, 1.f}, // log_level::critical
		},
	};

}
