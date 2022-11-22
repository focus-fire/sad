#pragma once

#include <Core/EventHandler.h>

namespace sad
{
	enum class EEngineMode
	{
		Editor, // Editor inputs allowed (moving a floating camera around, moving objects, etc.
		Game    // Game should start and editor actions should be killed
	};

	/**
	 * @brief Holds the current state of the engine 
	*/
	class EngineStateManager
	{
	public:
		EngineStateManager(EEngineMode mode = EEngineMode::Editor);

		/**
		 * @brief Retrieves the current engine mode 
		*/
		EEngineMode GetEngineMode() { return m_EngineMode; }

		/**
		 * @brief Toggles the current engine mode between Editor and Game
		*/
		void ToggleEngineMode();

		void OnToggleCamera();

	private:
		EEngineMode m_EngineMode;
	};
}