#include "sadpch.h"

#include "Application.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Game/Time.h>
#include <Game/Application.h>

#include "ECS/Registry.h"

#include "Renderer/RenderBuddy.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/ShaderResource.h"
#include "Renderer/Sample/Cube.h"

#include "AudioManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "InputManager.h"
#include "RenderableObject.h"
#include "EngineStateManager.h"
#include "LevelManager.h"

sad::Window* sad::Application::s_MainWindow;
sad::EngineStateManager* sad::Application::s_EngineState;

sad::Application::Application()
{
	s_MainWindow = new sad::Window();
	s_MainWindow->Start();
	s_MainWindow->CreateGLContext();

	s_EngineState = new EngineStateManager();

	m_Editor = new cap::Editor;
}

sad::Application::~Application()
{
	delete s_MainWindow;
	delete s_EngineState;
	delete m_Editor;
	
	// Allocated in LevelManager::ImportLevel()
	delete m_CurrentLevel;
}

void sad::Application::EngineStart()
{
	// Launch editor 
	m_Editor->Start();

	// Import Resources
	ResourceManager::Import();

	// Initialize the renderer and save a pointer to the FrameBuffer for the editor
	rad::RenderBuddy::Start();

	// Initialize Scripting
	cs::ScriptingEngine::Start();
	
	// Import Level and GUIDs 
	m_CurrentLevel = LevelManager::ImportLevel();
	SAD_ASSERT(m_CurrentLevel, "Failed to load a level");
	m_CurrentLevel->Start();

	bool isWindowClosed = false;

	// TODO: This is a nuclear bomb, make it safer
	//		 To update scripts offthread, mono has to be told where to invoke things...
	//std::thread gameThread = std::thread([&]() 
	//{
	//	while (!isWindowClosed)
	//	{
	//		if (s_EngineState->GetEngineMode() == EEngineMode::Game)
	//		{
	//			// Game Update
	//			float dt = pog::Time::GetDeltaTime();
	//			this->Update(dt);
	//		}
	//	}
	//});

	while (!isWindowClosed) 
	{
		PollEvents(isWindowClosed);

		float dt = pog::Time::GetDeltaTime();

		// Game Update
		if (s_EngineState->GetEngineMode() == EEngineMode::Game)
		{
			// TODO: When stop button is implemented, this should revert to 'false' on stop
			//		 This way, entities with scripts can be 'Awakened' each time the game is actually started 
			if (!m_IsGameOn)
			{
				this->Start();
				m_IsGameOn = true;
			}

			// Only start updating the game if `Start` has been called`
			if (m_IsGameOn)
				this->Update(dt);
		}

		// Engine Update
		sad::Application::Update(dt);
	}

	// gameThread.join();

	Teardown();
}

void sad::Application::PollEvents(bool& isWindowClosed)
{
	InputManager& input = InputManager::GetInstance();
	SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{
		m_Editor->CatchSDLEvents(event);
		input.CatchMouseEvents(event);
		input.CatchKeyboardEvents(event);
    
		if (input.IsControllerConnected())
			input.CatchControllerEvents(event);

		if (event.type == SDL_QUIT) 
			isWindowClosed = true;

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_MainWindow->GetSDLWindow()))
			isWindowClosed = true;

		if (event.type == SDL_CONTROLLERDEVICEADDED)
			input.OnControllerConnected(event.cdevice);

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
			input.OnControllerDisconnected();

		if (event.type == SDL_MOUSEMOTION) 
			input.SetMousePosition(event.motion.x, event.motion.y);
	}
}

void sad::Application::Update(float dt)
{
	// First 'pass' sets up the framebuffer
	// This clear color is the background for the game
	rad::RenderBuddy::ClearColor(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
	m_Editor->Clear();

	// Capture the current render in the framebuffer 
	rad::RenderBuddy::BindFrameBuffer();

	// Second 'pass' to recolor outside the framebuffer
	rad::RenderBuddy::ClearColor(glm::vec4(0.45f, 0.55f, 0.60f, 1.0f));

	// Update events subscribed to the update loop
	core::UpdateEvents();

	// Update current level context
	// Rendering and ecs systems
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();
	m_CurrentLevel->Update(world);

	// Unbind framebuffer for next pass
	rad::RenderBuddy::UnbindFrameBuffer();

	// Render Editor
	m_Editor->RenderGameWindow(rad::RenderBuddy::GetFrameBufferTexture());
	m_Editor->Render();

	// Render Window
	s_MainWindow->Render();
}

void sad::Application::Teardown()
{ 
	sad::cs::ScriptingEngine::RuntimeStop();
	sad::cs::ScriptingEngine::Teardown();

	LevelManager::ExportLevel();

	m_Editor->Teardown();

	s_MainWindow->Teardown();
}

glm::mat4 sad::Application::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::mat4 sad::Application::GetViewMatrix()
{
	return glm::lookAt(
		glm::vec3(0.5f, 2.5f, -3.0f), // Camera position
		glm::vec3(0.0f, -0.5f, 2.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);
}

glm::mat4 sad::Application::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(60.0f), s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
}
