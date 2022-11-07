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
		/**
		 * @brief 
		 * @param mode 
		*/
		EngineStateManager(EEngineMode mode = EEngineMode::Editor);

		/**
		 * @brief 
		 * @return 
		*/
		EEngineMode GetEngineMode() { return m_EngineMode; }

		/**
		 * @brief 
		 * @param mode 
		 * @return 
		*/
		EEngineMode SetEngineMode(EEngineMode mode) { m_EngineMode = mode; }

		/**
		 * @brief 
		*/
		void ToggleEngineMode();

	private:
		EEngineMode m_EngineMode;
	};
}