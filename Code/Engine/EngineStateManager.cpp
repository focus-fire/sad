#include "sadpch.h"
#include "EngineStateManager.h"
#include "Engine/EngineMode.h"

void sad::EngineStateManager::ToggleEngineMode()
{
	core::Log(ELogType::Info, "engine was switch to {}", static_cast<int>(m_EngineMode));
	sad::EngineStateManager::m_EngineMode = 
		(m_EngineMode == EngineMode::Editor ? EngineMode::Game : EngineMode::Editor);
}
