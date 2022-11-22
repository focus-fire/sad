#pragma once 

#include <thread>

#include <Editor/Editor.h>

#include "ECS/Entity.h"
#include "Renderer/Renderer.h"
#include "Scripting/ScriptingEngine.h"

#include "Level.h"
#include "Window.h"
#include "RenderableResource.h"
#include "AudioResource.h"
#include "EngineStateManager.h"
#include "Engine/EditorCamera.h"
#include "Game/GameCamera.h"


struct SDL_Window;

namespace sad
{
	/**
	 * @brief Base class for all lifecycle specific methods for the Engine/Game projects
	*/
	class Application
	{
	public:
		explicit Application();
		virtual ~Application();

		/**
		 * @brief Entrypoint for the engine application that initializes systems
		*/
		void EngineStart();

		/**
		 * @brief Resets the level back to the last saved instance
		*/
		void LevelReset();

		/**
		 * @brief Starting point for applications aside from the engine
		*/
		virtual void Start() = 0;

		/**
		 * @brief Lifecycle method that polls for window events and submits events to required systems
		*/
		void PollEvents(bool& isClosed);

		/**
		 * @brief Main update loop for the Engine
		*/
		virtual void Update(float dt);

		/**
		 * @brief Destruction method that tears down the services associated with the engine 
		*/
		virtual void Teardown();

	public:
		static Window* s_MainWindow;
		static EngineStateManager* s_EngineState;

		static float s_DeltaTime;

		static EditorCamera* s_EditorCamera;

		static GameCamera* s_GameCamera;

	private:
		cap::Editor* m_Editor = nullptr;
		Level* m_CurrentLevel;

		bool m_IsGameOn = false;
	};
}