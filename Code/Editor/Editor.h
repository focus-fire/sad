#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <Engine/Window.h>
#include <Engine/Level.h>

#include "DebugTerminal.h"
#include "GizmoSystem.h"

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
		 * @param frameBufferTextureId MeshTexture Id in OpenGL for the attached framebuffer that the game is being rendered to
		*/
		void RenderGameWindow(unsigned int frameBufferTextureId);
		void Render();

		// TODO: Maybe make an ImGuiHelpers class with static returns for things like this? 
		bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);
		void EditorControls();

		void Teardown();

	private:
		void EntityListPanel();

		void ActionPanel();
			void PlayButton();
			void PauseButton();
			void StopButton();
			void SaveButton();

		sad::Level* m_CurrentLevelContext = nullptr;
		DebugTerminal* m_DebugTerminal = nullptr;
		GizmoSystem* m_GizmoSystem = nullptr;

		ImFont* m_NotoSans = nullptr;
		ImFont* m_Default = nullptr;

		bool m_ShowGameWindow;
		float m_GameWindowWidth;
		float m_GameWindowHeight;
		std::string m_GameWindowTitle;

		bool m_CanEditorBePaused;
		bool m_CanEditorBeStopped;
		bool m_CanEditorBePlayed;
	};
}
