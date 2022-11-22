#include "sadpch.h"

#include "EngineStateManager.h"
#include "Engine/Renderer/RenderBuddy.h"
#include "Engine/Camera.h"
#include "Application.h"

sad::EngineStateManager::EngineStateManager(EEngineMode mode /* = EEngineMode::Editor */)
	: m_EngineMode(mode)
{
	std::function<void(void)> toggleEngineFunction = std::bind(&EngineStateManager::ToggleEngineMode, this);
	std::function<void(void)> onToggleCamera = std::bind(&EngineStateManager::OnToggleCamera, this);
	core::InitializeListener("OnToggleEngineMode", toggleEngineFunction);
	core::InitializeListener("OnToggleEngineMode", onToggleCamera);
}

void sad::EngineStateManager::ToggleEngineMode()
{
	sad::EngineStateManager::m_EngineMode = m_EngineMode == EEngineMode::Editor ? EEngineMode::Game : EEngineMode::Editor;
	core::Log(ELogType::Trace, "Engine was switched to {}", m_EngineMode == EEngineMode::Editor ? "Editor" : "Game");
}

void sad::EngineStateManager::OnToggleCamera()
{
	if (m_EngineMode == EEngineMode::Editor)
	{
		sad::rad::RenderBuddy::SetCameraInstance(sad::Application::s_EditorCamera);
	}
	if (m_EngineMode == EEngineMode::Game)
	{
		sad::rad::RenderBuddy::SetCameraInstance(sad::Application::s_GameCamera);
	}
}
