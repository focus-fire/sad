#pragma once

#include <SDL2/SDL.h>

#include <Engine/Window.h>

/*
 * "That editor is cap!"
 */

namespace cap 
{
	class Cap
	{
	public:
		explicit Cap(sad::Window* mainWindow);

		void Start(SDL_Window* sdlWindow, SDL_GLContext glContext);
		void CatchSDLEvents(const SDL_Event& event);
		void Clear();

		// TODO: Ideally abstract these parameters out or modify the logic with events
		//		 OR abstract this into it's own GameWindowPanel
		void RenderGameWindow(unsigned int frameBufferTextureId);
		void Render();

		void Teardown();

	private:
		sad::Window* m_MainWindow = nullptr;

		bool m_ShowGameWindow;
		bool m_ShowWelcomeWindow;

		float m_GameWindowWidth;
		float m_GameWindowHeight;
	};
}