#pragma once 

#include <thread>

#include <Editor/Editor.h>

#include "ECS/Entity.h"
#include "Renderer/Renderer.h"
#include "Scripting/ScriptingEngine.h"

#include "Window.h"
#include "RenderableResource.h"
#include "AudioResource.h"
#include "EngineStateManager.h"

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

		// TODO: Abstract view projection matrix into camera
		static glm::mat4 GetViewProjectionMatrix();
		static glm::mat4 GetViewMatrix();
		static glm::mat4 GetProjectionMatrix();

	public:
		static Window* s_MainWindow;
		static EngineStateManager* s_EngineState;

	private:
		cs::ScriptingEngine* m_ScriptingEngine = nullptr;
		cap::Editor* m_Editor = nullptr;

		bool m_PlayMusic;
		AudioResource* m_MusicResource;
	};
}