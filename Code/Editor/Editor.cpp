#include "sadpch.h"

#include "Editor.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

#include <Engine/Application.h>

const char* UI_TEXT = "Welcome to the sadEngine!";
int m_Counter = 0;

void edit_text()
{
	m_Counter++;
	UI_TEXT = "Welcome to the game! " + char(m_Counter);
}

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

void cap::Editor::Start(SDL_Window* sdlWindow, SDL_GLContext glContext)
{
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
	ImGui_ImplOpenGL3_Init("#version 150");

	// Create test event
	core::InitializeListener("UI", edit_text, 0, false);
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
	core::SignalEvent("UI");
	if (m_ShowWelcomeWindow)
	{
		ImGui::Begin("Welcome", &m_ShowWelcomeWindow);  
		ImGui::SetWindowPos(ImVec2(60.0f, 790.0f), ImGuiCond_Once);
		ImGui::Text(UI_TEXT);
		
		if (ImGui::Button("Close"))
			m_ShowWelcomeWindow = false;

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
