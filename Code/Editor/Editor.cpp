#include "sadpch.h"

#include "Editor.h"

#include <glm/gtc/type_ptr.hpp>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <imterm/terminal.hpp>
#include <ImGuizmo.h>
#include <entt/entt.hpp>

#include <Engine/Application.h>
#include <Engine/Camera.h>
#include <Engine/LevelManager.h>
#include <Engine/ResourceManager.h>
#include <Engine/FontResource.h>

#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/NameComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>

#include <Game/Time.h>

cap::Editor::Editor()
	: m_CurrentLevelContext(nullptr)
	, m_DebugTerminal(new cap::DebugTerminal())
	, m_GizmoSystem(new cap::GizmoSystem())
	, m_ShowGameWindow(true)
	, m_GameWindowTitle("Default - sadEngine")
	, m_CanEditorBePaused(false)
	, m_CanEditorBePlayed(true)
	, m_CanEditorBeStopped(false)
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
	// Configure editor fonts
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Load Noto Sans font
	sad::FontResource* notoSansFont = sad::ResourceManager::GetResource<sad::FontResource>("NotoSans-Regular.ttf");
	m_NotoSans = io.Fonts->AddFontFromFileTTF(notoSansFont->GetResourceAbsolutePath().c_str(), 16.0f);
	SAD_ASSERT(m_NotoSans, "Failed to load Editor font from ResourceManager");

	m_Default = io.Fonts->AddFontDefault();
	SAD_ASSERT(m_Default, "Failed to load default ImGui font");

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

const float c_RightColumnWidth = 250.0f;
const float c_WindowTopEdge = 0.0f;
const float c_WindowLeftEdge = 0.0f;
const float c_GameWindowX = c_WindowLeftEdge;
const float c_GameWindowY = c_WindowTopEdge;
const float c_GameWindowRightEdge = 1025.0f;
const float c_PlayWindowY = c_WindowTopEdge;
const float c_PlayWindowHeight = 100.0f;
const float c_ListWindowY = c_PlayWindowHeight;
const float c_ListWindowHeight = 355.0f;

void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.13f, 0.13f, 0.13f));

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
	ImGui::SetWindowPos(ImVec2(c_GameWindowX, c_GameWindowY), ImGuiCond_Always);

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
	ImGui::PushFont(m_Default);
	m_DebugTerminal->Render();
	ImGui::PopFont();

	// If the editor cannot be 'played' it is running
	pog::Time::TimeScale = !m_CanEditorBePlayed ? 1.0f : 0.0f;
	
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
		// Only allow ctl+S to save when the editor hasn't been started
		if (m_CanEditorBePlayed && !m_CanEditorBeStopped && ImGui::IsKeyPressed(ImGuiKey_S))
			sad::LevelManager::ExportLevel();

		if (ImGui::IsKeyPressed(ImGuiKey_P))
		{
			m_CanEditorBePlayed = !m_CanEditorBePlayed;
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
	const float listWindowX = sad::Application::s_MainWindow->GetWidth() - c_RightColumnWidth;
	ImGui::SetWindowPos(ImVec2(listWindowX, c_ListWindowY), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(c_RightColumnWidth, c_ListWindowHeight), ImGuiCond_Always);

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
	const float playWindowX = sad::Application::s_MainWindow->GetWidth() - c_RightColumnWidth;
	ImGui::SetWindowPos(ImVec2(playWindowX, c_PlayWindowY), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(c_RightColumnWidth, c_PlayWindowHeight), ImGuiCond_Always);

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
	if (!m_CanEditorBePlayed && m_CanEditorBePaused)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	// Current editor mode, "Pause" or "Play"
	if (ButtonCenteredOnLine("Play") && m_CanEditorBePlayed  && !m_CanEditorBePaused)
	{
		// Ensure that each time the editor toggles INTO play mode, the level is exported
		// This way, if the editor can be stopped (it is running) the level doesn't exit when they resume play
		if (!m_CanEditorBeStopped)
			sad::LevelManager::ExportLevel();

		// Only allow toggling the play mode if the editor can be paused
		m_CanEditorBePlayed = false;
		m_CanEditorBePaused = true;
		m_CanEditorBeStopped = true;

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
		m_CanEditorBePlayed = true;
		m_CanEditorBeStopped = true;

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
	if (!m_CanEditorBeStopped)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	if (ButtonCenteredOnLine("Stop") && m_CanEditorBeStopped)
	{
		// When the editor is in pause it's already using the editor camera/controls
		// Therefore, the engine mode only needs to actually be toggled when 'Stop' is pressed from the game
		if (m_CanEditorBePaused)
			core::SignalEvent("OnToggleEngineMode");

		// Stop the editor and reset available buttons
		m_CanEditorBePlayed = true;
		m_CanEditorBePaused = false;
		m_CanEditorBeStopped = false;

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

	// Only allow saving if the editor is outside of game mode
	if (m_CanEditorBeStopped)
	{
		disabled = true;
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	if (ButtonCenteredOnLine("Save") && !m_CanEditorBeStopped)
		sad::LevelManager::ExportLevel();

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}
