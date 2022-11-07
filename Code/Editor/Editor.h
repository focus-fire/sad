#pragma once

#include <SDL2/SDL.h>

#include <Engine/Window.h>

#include "DebugTerminal.h"

/*
 * "That editor is cap!"
 */

namespace cap 
{
	class Editor
	{
	public:
		explicit Editor();
		~Editor();

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
		void Render();

		void Teardown();

	private:
		DebugTerminal* m_DebugTerminal;

		bool m_ShowGameWindow;
		float m_GameWindowWidth;
		float m_GameWindowHeight;

		bool m_IsEditorInPlayMode;
	};
}
