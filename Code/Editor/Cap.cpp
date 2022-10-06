#include "sadpch.h"

#include "Cap.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

cap::Cap::Cap(sad::Window* mainWindow)
	: m_MainWindow(mainWindow)
	, m_ShowGameWindow(true)
	, m_ShowWelcomeWindow(true)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	m_GameWindowWidth = static_cast<float>(m_MainWindow->GetWidth()) / 2.0f;
	m_GameWindowHeight = static_cast<float>(m_MainWindow->GetHeight()) / 2.0f;
}

void cap::Cap::Start(SDL_Window* sdlWindow, SDL_GLContext glContext)
{
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void cap::Cap::CatchSDLEvents(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void cap::Cap::Clear()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void cap::Cap::RenderGameWindow(unsigned int frameBufferTextureId)
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

void cap::Cap::Render()
{
	if (m_ShowWelcomeWindow)
	{
		ImGui::Begin("Welcome", &m_ShowWelcomeWindow);  
		ImGui::SetWindowPos(ImVec2(60.0f, 525.0f), ImGuiCond_Once);
		ImGui::Text("Welcome to the sadEngine!");
		
		if (ImGui::Button("Close"))
			m_ShowWelcomeWindow = false;

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cap::Cap::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
