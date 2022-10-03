#include "sadpch.h"

#include "Application.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

// Idea for a suitable GL_CALL macro?
// #define SAD_GL_CALL(x) sad::gl::ClearErrors(); x; sad::gl::ErrorLog();

sad::Application::Application()
	: m_Window(new Window())
{ 
	m_Window->Start();
	m_Window->CreateGLContext();
}

sad::Application::~Application()
{
	delete m_Window;
}

void sad::Application::Start()
{
	SDL_Window* sdlWindow = m_Window->GetSDLWindow();
	SDL_GLContext glContext = m_Window->GetGLContext();
	
	// Setup ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
	ImGui_ImplOpenGL3_Init("#version 150");

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);

	bool isClosed = false;
	SDL_Event event;

	while (!isClosed) 
	{
		while (SDL_PollEvent(&event)) 
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) 
				isClosed = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdlWindow))
				isClosed = true;
		}

		// Setup ImGui frames
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Render demo ImGui window
		bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);
		ImGui::Render();

		GL_CALL(glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight()));
		GL_CALL(glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(sdlWindow);
	}

	m_Window->Teardown();
	Teardown();
}

void sad::Application::Teardown()
{
	// ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
