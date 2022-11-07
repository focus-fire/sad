#include "sadpch.h"

#include "EngineStateManager.h"

sad::EngineStateManager::EngineStateManager(EEngineMode mode /* = EEngineMode::Editor */)
	: m_EngineMode(mode)
{
	std::function<void(void)> toggleEngineFunction = std::bind(&EngineStateManager::ToggleEngineMode, this);
	core::InitializeListener("OnToggleEngineMode", toggleEngineFunction);
}

void sad::EngineStateManager::ToggleEngineMode()
{
	core::Log(ELogType::Trace, "Engine was switched to {}", m_EngineMode == EEngineMode::Editor ? "Editor" : "Game");
	sad::EngineStateManager::m_EngineMode = m_EngineMode == EEngineMode::Editor ? EEngineMode::Game : EEngineMode::Editor;
}
