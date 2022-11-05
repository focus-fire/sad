#pragma once 

#include <Editor/Editor.h>

#include "ECS/Entity.h"
#include "Renderer/Renderer.h"

#include "Window.h"
#include "RenderableResource.h"
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
		void PollEvents(bool* isClosed);

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
		static EngineStateManager* s_EngineStateManager;

	private:
		sad::rad::Renderer* m_Renderer = nullptr;
		cap::Editor* m_Editor = nullptr;

		//// Temporary Variables for Test Level
		//RenderableResource* m_CubeResource;
		//sad::ecs::Entity m_FirstCubeEntity;
		//sad::ecs::Entity m_SecondCubeEntity;

		glm::mat4 m_VpMatrix;

		//float m_CubeTranslate;
		//std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
	};
}