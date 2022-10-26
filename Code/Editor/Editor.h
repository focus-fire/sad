#pragma once

#include <SDL2/SDL.h>

#include <Engine/Window.h>

/*
 * "That editor is cap!"
 */

namespace cap 
{
	class Editor
	{
	public:
		explicit Editor();

		void Start();
		void CatchSDLEvents(const SDL_Event& event);
		void Clear();

		// TODO: Ideally abstract these parameters out or modify the logic with events
		//		 OR abstract this into it's own GameWindowPanel

		/**
		 * @brief Renders a panel with the game's current render embedded into it 
		 * @param frameBufferTextureId Texture Id in OpenGL for the attached framebuffer that the game is being rendered to
		*/
		void RenderGameWindow(unsigned int frameBufferTextureId);
		void Render(unsigned int frameBufferTextureId);

		void Teardown();

	private:
		bool m_ShowGameWindow;
		bool m_ShowWelcomeWindow;

		float m_GameWindowWidth;
		float m_GameWindowHeight;

		bool m_IsEditorInPlayMode;
	};
}