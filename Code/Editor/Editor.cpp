#include "sadpch.h"

#include "Editor.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <imterm/terminal.hpp>
#include <ImGuizmo.h>

#include <Engine/Application.h>

#include <Game/Time.h>

cap::Editor::Editor()
	: m_DebugTerminal(new cap::DebugTerminal())
	, m_ShowGameWindow(true)
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
	ImGui::Begin("level.json - sadEngine", &showGameWindow, 0);
	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth, m_GameWindowHeight), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);

	// Pass frameBuffer texture to be rendered in window
	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));

	RenderGizmos();

	ImGui::End();
}

void cap::Editor::RenderGizmos()
{
	static ImGuizmo::MODE currentGizmoMode(ImGuizmo::LOCAL);

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::Enable(true);

	ImGuizmo::SetRect(0, 0, m_GameWindowWidth, m_GameWindowHeight);

	glm::mat4 viewMatrix = sad::Application::GetViewMatrix();
	glm::mat4 projectionMatrix = sad::Application::GetProjectionMatrix();
	ImGuizmo::DrawGrid(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
}

void cap::Editor::Render()
{
	m_DebugTerminal->Render();

	const char* currentMode = m_IsEditorInPlayMode ? "Pause" : "Play";
	pog::Time::TimeScale = m_IsEditorInPlayMode ? 1.0f : 0.0f;

	ImGui::Begin("Action Panel");
	ImGui::SetWindowPos(ImVec2(1350.0f, 35.0f), ImGuiCond_Once);
	if (ImGui::Button(currentMode))
	{
		m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
		core::SignalEvent("OnToggleEngineMode");
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
