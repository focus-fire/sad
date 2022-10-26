#include "sadpch.h"

#include "Editor.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <imterm/terminal.hpp>

#include <Engine/Application.h>

cap::Editor::Editor()
	: m_DebugTerminal(new cap::DebugTerminal())
	, m_ShowGameWindow(true)
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
}

void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::Begin("level.json - sadEngine", &showGameWindow);
	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth, m_GameWindowHeight), ImGuiCond_Once);
	ImVec2 availableSize = ImGui::GetContentRegionAvail();

	// Pass frameBuffer texture to be rendered in window
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void cap::Editor::Render()
{
	m_DebugTerminal->Render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
