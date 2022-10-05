#include "sadpch.h"

#include "Cap.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

cap::Cap::Cap()
	: m_ShowWelcomeWindow(true)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
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

void cap::Cap::RenderGameWindow(unsigned int frameBufferTextureId, unsigned int width, unsigned int height)
{
	bool showGameWindow = true;
	ImGui::Begin("level.json - sadEngine", &showGameWindow);
	
	// Set the starting window size
	ImGui::SetWindowSize(ImVec2(width + 15.0f, height + 35.0f), 0);

	// Add the rendered image of the game to the panel 
	// Use the upper left/lower right corners to ensure that image remains in the frame
	ImVec2 uvUpperLeftCorner = ImGui::GetCursorScreenPos();
	ImVec2 uvLowerRightCorner = ImVec2(uvUpperLeftCorner.x + width, uvUpperLeftCorner.y + height);

	// Pass frameBuffer texture to window
	ImGui::GetWindowDrawList()->AddImage(INT_TO_VOIDP(frameBufferTextureId),
		uvUpperLeftCorner,
		uvLowerRightCorner,
		ImVec2(0, 1),
		ImVec2(1, 0));

	ImGui::End();
}

void cap::Cap::Render()
{
	// Welcome Window
	if (m_ShowWelcomeWindow)
	{
		ImGui::Begin("Welcome", &m_ShowWelcomeWindow);  
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
