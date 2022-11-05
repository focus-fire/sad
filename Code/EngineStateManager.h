#pragma once
#include "Engine/EngineMode.h"

namespace sad
{
	class EngineStateManager
	{
	public:
		EngineStateManager(EngineMode mode) {
			m_EngineMode = mode;
		}
		/**
		*  return editor or game mode
		*/
		EngineMode GetEngineMode() { return m_EngineMode; }

		/**
		*  set to either editor or game
		*/
		EngineMode SetEngineMode(EngineMode mode) { m_EngineMode = mode; }

		void ToggleEngineMode();

	private:
		EngineMode m_EngineMode;
	};
}