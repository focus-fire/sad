#include "sadpch.h"

#include "Editor.h"

#include <glm/gtc/type_ptr.hpp>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <imterm/terminal.hpp>
#include <ImGuizmo.h>
#include <entt/entt.hpp>

#include <Engine/Application.h>
#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/NameComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/Camera.h>
#include <Engine/LevelManager.h>

#include <Game/Time.h>

cap::Editor::Editor()
	: m_CurrentLevelContext(nullptr)
	, m_DebugTerminal(new cap::DebugTerminal())
	, m_GizmoSystem(new cap::GizmoSystem())
	, m_ShowGameWindow(true)
	, m_GameWindowTitle("Default - sadEngine")
	, m_IsEditorInPlayMode(false)
	, m_CanEditorBePaused(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	m_GameWindowWidth = static_cast<float>(sad::Application::s_MainWindow->GetWidth()) / 1.25f;
	m_GameWindowHeight = static_cast<float>(sad::Application::s_MainWindow->GetHeight()) / 1.25f;
}

cap::Editor::~Editor()
{
	delete m_DebugTerminal;
	delete m_GizmoSystem;
}

void cap::Editor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(sad::Application::s_MainWindow->GetSDLWindow(), sad::Application::s_MainWindow->GetGLContext());
	ImGui_ImplOpenGL3_Init("#version 150");

	m_DebugTerminal->Start();
}

void cap::Editor::CatchSDLEvents(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void cap::Editor::Clear()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

const float rightColumnWidth = 250.0f;
const float windowTopEdge = 0.0f;
const float windowLeftEdge = 0.0f;
const float gameWindowX = windowLeftEdge;
const float gameWindowY = windowTopEdge;
const float gameWindowRightEdge = 1025.0f;
const float playWindowX = gameWindowRightEdge;
const float playWindowY = windowTopEdge;
const float playWindowHeight = 65.0f;
const float listWindowX = gameWindowRightEdge;
const float listWindowY = playWindowHeight;
const float listWindowHeight = 390.0f;
const float transformWindowX = gameWindowRightEdge;
const float transformWindowY = listWindowY + listWindowHeight;
const float transformWindowHeight = 125.0f;


void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.83f, 0.83f, 0.83f));

	ImGui::Begin(m_GameWindowTitle.c_str(), &showGameWindow, m_GizmoSystem->s_GameWindowFlags);
	ImGui::PopStyleColor();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow))
	{
		core::SignalEvent("OnInputUnlock");
	}
	else
	{
		core::SignalEvent("OnInputLock");
	}

	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth / 1.25, m_GameWindowHeight / 1.25), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(gameWindowX, gameWindowY), ImGuiCond_Once);

	// Pass frameBuffer texture to be rendered in window
	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));

	// Rendering gizmos must be completed while framebuffer is still being rendered
	// This allows the grid and all gizmos to properly appear in the game window
	if (sad::Application::s_EngineState->GetEngineMode() == sad::EEngineMode::Editor)
	{
		m_GizmoSystem->Render();
	}

	ImGui::End();
}

void cap::Editor::Render()
{
	m_DebugTerminal->Render();
	pog::Time::TimeScale = m_IsEditorInPlayMode ? 1.0f : 0.0f;
	
	// Renderable Panels
	ActionPanel();
	EntityListPanel();
	EditorControls();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


bool cap::Editor::ButtonCenteredOnLine(const char* label, float alignment /* = 0.5f */)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

/**
 * Controls
 * ctrl+s : Save state
 * ctrl+b : Switch editor mode
*/
void cap::Editor::EditorControls()
{
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (!m_IsEditorInPlayMode && ImGui::IsKeyPressed(ImGuiKey_S))
		{
			sad::LevelManager::ExportLevel();
			m_IsEditorInPlayMode = false;
			core::Log(ELogType::Trace, "Saved");
		}

		if (ImGui::IsKeyPressed(ImGuiKey_P))
		{
			m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
			core::SignalEvent("OnToggleEngineMode");
		}
	}
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void cap::Editor::EntityListPanel()
{
	// Really scuffed way to list entities, included mainly for debugging
	// Cycles through available names and lists them in the editor
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos(ImVec2(listWindowX, listWindowY), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(rightColumnWidth, listWindowHeight), ImGuiCond_Once);

	auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::NameComponent>();
	for (auto [entity, name] : view.each())
	{
		ImGui::Text(name.m_Name.c_str());
	}

	ImGui::End();
}

void cap::Editor::ActionPanel()
{
	ImGui::Begin("Action Panel");
	ImGui::SetWindowPos(ImVec2(playWindowX, playWindowY), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(rightColumnWidth, playWindowHeight), ImGuiCond_Once);

	// Play button for the editor, starts the game time
	PlayButton();

	// Pause button for the editor, pauses the game time and pops the player into editor mode
	PauseButton();

	// Stop button for the editor, triggers a level reset
	StopButton();

	// Basic save button for the editor, only applicable outside of play mode
	SaveButton();

	ImGui::End();
}

void cap::Editor::PlayButton()
{
	bool disabled = false;
	if (m_IsEditorInPlayMode && m_CanEditorBePaused)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	// Current editor mode, "Pause" or "Play"
	if (ButtonCenteredOnLine("Play") && !m_IsEditorInPlayMode  && !m_CanEditorBePaused)
	{
		// Ensure that each time the editor toggles INTO play mode, the level is exported
		sad::LevelManager::ExportLevel();

		// Only allow toggling the play mode if the editor can be paused
		m_CanEditorBePaused = true;

		// Start the game 
		m_IsEditorInPlayMode = true;

		core::Log(ELogType::Debug, "Started the game...");

		core::SignalEvent("OnToggleEngineMode");
	}

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}

void cap::Editor::PauseButton()
{
	bool disabled = false;
	if (!m_CanEditorBePaused)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	if (ButtonCenteredOnLine("Pause") && m_CanEditorBePaused)
	{
		// Allow the editor to be played again
		m_CanEditorBePaused = false;
		m_IsEditorInPlayMode = false;

		core::Log(ELogType::Debug, "Paused the game...");

		core::SignalEvent("OnToggleEngineMode");
	}

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}

void cap::Editor::StopButton()
{
	bool disabled = false;
	if (!m_IsEditorInPlayMode)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	if (ButtonCenteredOnLine("Stop") && m_IsEditorInPlayMode)
	{
		// When the editor is in pause it's already using the editor camera/controls
		// Therefore, the engine mode only needs to actually be toggled when 'Stop' is pressed from the game
		if (m_CanEditorBePaused)
			core::SignalEvent("OnToggleEngineMode");

		m_IsEditorInPlayMode = false;
		m_CanEditorBePaused = false;

		core::Log(ELogType::Debug, "Stopped the game...");
		core::SignalEvent("ResetLevel");
	}

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}

void cap::Editor::SaveButton()
{
	bool disabled = false;
	if (m_IsEditorInPlayMode)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	if (ButtonCenteredOnLine("Save") && !m_IsEditorInPlayMode)
		sad::LevelManager::ExportLevel();

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}
