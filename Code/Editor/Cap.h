#pragma once

#include <SDL2/SDL.h>

/*
 * "That editor is cap!"
 */

namespace cap 
{
	class Cap
	{
	public:
		Cap();

		void Start(SDL_Window* sdlWindow, SDL_GLContext glContext);
		void CatchSDLEvents(const SDL_Event& event);
		void Clear();

		// TODO: Ideally abstract these parameters out or modify the logic with events
		//		 OR abstract this into it's own GameWindowPanel
		void RenderGameWindow(unsigned int frameBufferTextureId, unsigned int width, unsigned int height);
		void Render();

		void Teardown();

	private:
		bool m_ShowWelcomeWindow;
	};
}