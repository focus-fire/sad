#include "sadpch.h"

#include "Editor.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

#include <Engine/Application.h>

cap::Editor::Editor()
	: m_ShowGameWindow(true)
	, m_ShowWelcomeWindow(true)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	m_GameWindowWidth = static_cast<float>(sad::Application::s_MainWindow->GetWidth()) / 1.25f;
	m_GameWindowHeight = static_cast<float>(sad::Application::s_MainWindow->GetHeight()) / 1.25f;
}

void cap::Editor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(sad::Application::s_MainWindow->GetSDLWindow(), sad::Application::s_MainWindow->GetGLContext());
	ImGui_ImplOpenGL3_Init("#version 150");
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
	if (m_ShowWelcomeWindow)
	{
		ImGui::Begin("Welcome", &m_ShowWelcomeWindow);  
		ImGui::SetWindowPos(ImVec2(60.0f, 790.0f), ImGuiCond_Once);
		ImGui::Text("Welcome to the sadEngine!");
		
		if (ImGui::Button("Close"))
			m_ShowWelcomeWindow = false;

		ImGui::End();
	}

	ImGui::Begin("Controls");
	ImGui::SetWindowSize(ImVec2(300.0f, 100.0f));
	ImGui::SetWindowPos(ImVec2(260.0f, 790.0f), ImGuiCond_Once);
	ImGui::Text("Move:   WASD");
	ImGui::Text("Fly:    space/shift");
	ImGui::Text("Rotate: left/right arrows");
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
