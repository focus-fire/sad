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
		DebugTerminalHelper();

		/**
		 * @brief Specifies action taken by terminal for autocompleting user results
		*/
		static std::vector<std::string> NoCompletion(argument_type& arg);

		/**
		 * @brief Clears the terminal output
		 *
		 * @usage clear 
		*/
		static void Clear(argument_type& arg);

		/**
		 * @brief Prints to the terminal through the debug logging sinks 
		 *
		 * @usage debug <message>
		*/
		static void Debug(argument_type& arg);

		/**
		 * @brief Closes the currently focused terminal
		 *
		 * @usage close
		*/
		static void Close(argument_type& arg);

		/**
		 * @brief Instantiates an entity in the level with a provided name
		 *
		 * @usage instantiate <entity_name>
		*/
		static void Instantiate(argument_type& arg);

		/**
		 * @brief Instantiates an entity in the level with a provided mdoel name 
		 *
		 * @usage instantiate_model <entity_name> <model_name>
		*/
		static void InstantiateModel(argument_type& arg);

		/**
		 * @brief Instantiates an entity in the level with a PrimitiveResource attached to it
		 *
		 * @usage instantiate_shape <entity_name> <shape>
		*/
		static void InstantiateShape(argument_type& arg);

		/**
		 * @brief Instantiates an entity in the level with a RenderableSprite attached to it
		 *
		 * @usage instantiate_sprite <entity_name> <sprite_name>
		*/
		static void InstantiateSprite(argument_type& arg);
		
		/**
		 * @brief Destroys an entity in the level with a particular name 
		 *
		 * @usage destroy <entity_name>
		*/
		static void Destroy(argument_type& arg);

		/**
		 * @brief Binds a script to an entity in the level with a particular name
		 *
		 * @usage bind_script <entity_name> <script_name>
		*/
		static void BindScriptToEntity(argument_type& arg);

		/**
		 * @brief Unbinds a script from an entity in the level with a particular name
		 *
		 * @usage unbind_script <entity_name> <script_name>
		*/
		static void UnbindScriptFromEntity(argument_type& arg);

		/**
		 * @brief Adds a light into the scene
		 *
		 * @usage add_light <entity_name> 
		*/
		static void AddLight(argument_type& arg);
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
