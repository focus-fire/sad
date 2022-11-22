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

#include <Game/Time.h>

cap::Editor::Editor()
	: m_CurrentLevelContext(nullptr)
	, m_DebugTerminal(new cap::DebugTerminal())
	, m_GizmoSystem(new cap::GizmoSystem())
	, m_ShowGameWindow(true)
	, m_GameWindowTitle("Default - sadEngine")
	, m_IsEditorInPlayMode(false)
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

void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.83f, 0.83f, 0.83f));

	ImGui::Begin(m_GameWindowTitle.c_str(), &showGameWindow, m_GizmoSystem->s_GameWindowFlags);
	ImGui::PopStyleColor();

	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth / 1.25, m_GameWindowHeight / 1.25), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(50.0, 25.0), ImGuiCond_Once);

	// Pass frameBuffer texture to be rendered in window
	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));

	// TODO: Abstract into interaction mode class
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

	const char* currentMode = m_IsEditorInPlayMode ? "Pause" : "Play";
	pog::Time::TimeScale = m_IsEditorInPlayMode ? 1.0f : 0.0f;

	ImGui::Begin("Action Panel");
	ImGui::SetWindowPos(ImVec2(1150.0f, 35.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(125.0f, 65.0f), ImGuiCond_Once);
	if (ButtonCenteredOnLine(currentMode))
	{
		m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
		core::SignalEvent("OnToggleEngineMode");
	}
	ImGui::End();

	// Really scuffed way to list entities, included mainly for debugging
	// Cycles through available names and lists them in the editor
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos(ImVec2(1150.0f, 265.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(200.0f, 225.0f), ImGuiCond_Once);
	auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::NameComponent>();
	for (auto [entity, name] : view.each())
		ImGui::Text(name.m_Name.c_str());
	ImGui::End();

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

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
