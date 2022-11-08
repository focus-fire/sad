#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

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
		Editor();
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
		std::vector<glm::vec3> RenderGizmos(float* modelMatrix, bool transformDecomposition);
		void Render();

		// TODO: Maybe make an ImGuiHelpers class with static returns for things like this? 
		bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);

		void Teardown();

	private:
		DebugTerminal* m_DebugTerminal;

		ImGuiWindowFlags m_GameWindowFlags;
		int m_LastGizmoUsed = 0;

		bool m_ShowGameWindow;
		float m_GameWindowWidth;
		float m_GameWindowHeight;

		bool m_IsEditorInPlayMode;
	};
}
