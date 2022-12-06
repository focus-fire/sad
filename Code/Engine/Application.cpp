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
#include <Game/GameCamera.h>

#include "ECS/Registry.h"

#include "Renderer/RenderBuddy.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/ShaderResource.h"
#include "Renderer/Sample/Cube.h"
#include "Renderer/Skybox.h"

#include "ResourceManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "InputManager.h"
#include "EngineStateManager.h"
#include "LevelManager.h"
#include "Camera.h"

sad::Window* sad::Application::s_MainWindow;
sad::EngineStateManager* sad::Application::s_EngineState;
sad::GameCamera* sad::Application::s_GameCamera;
sad::EditorCamera* sad::Application::s_EditorCamera;
sad::ThreadQueue* sad::Application::s_ThreadQueue;

float sad::Application::s_DeltaTime;

sad::Application::Application()
{
	s_MainWindow = new sad::Window();
	s_MainWindow->Start();
	s_MainWindow->CreateGLContext();

	s_EngineState = new EngineStateManager();
	s_ThreadQueue = new ThreadQueue();

	s_EditorCamera = new sad::EditorCamera();
	s_GameCamera = new sad::GameCamera();

	
	// Set the default camera instance to the editor camera
	sad::rad::RenderBuddy::SetCameraInstance(s_EditorCamera);

	m_Editor = new cap::Editor;
	m_Skybox = new rad::Skybox();

	// Initialize event listener for detecting a reset/stop for the currently running game
	core::InitializeListener("ResetLevel", std::bind(&sad::Application::LevelReset, this));
}

sad::Application::~Application()
{
	delete s_MainWindow;
	delete s_EngineState;
	delete s_ThreadQueue;

	delete m_Editor;
	// Allocated in LevelManager::ImportLevel()
	delete m_CurrentLevel;
	delete m_Skybox;

	delete s_EditorCamera;
	delete s_GameCamera;
}

void sad::Application::EngineStart()
{
	// Import Resources
	ResourceManager::Import();

	// Launch editor 
	m_Editor->Start();

	// Initialize the renderer and save a pointer to the FrameBuffer for the editor
	rad::RenderBuddy::Start();

	// Skybox
	m_Skybox->SetSkybox();

	// Initialize Scripting
	cs::ScriptingEngine::Start();

	// Initialize Camera
	rad::RenderBuddy::GetCameraInstance()->Start();

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

		s_DeltaTime = pog::Time::GetDeltaTime();

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
			{
				this->Update(s_DeltaTime);
			}
		}

		// Engine Update
		sad::Application::Update(s_DeltaTime);
	}

	// gameThread.join();

	Teardown();
}

void sad::Application::LevelReset()
{
	// Stop the game
	m_IsGameOn = false;

	// Clear the registry
	sad::ecs::Registry::GetEntityWorld().clear();

	// Import Level and GUIDs 
	delete m_CurrentLevel;
	m_CurrentLevel = LevelManager::ImportLevel();
	SAD_ASSERT(m_CurrentLevel, "Failed to load a level");
	m_CurrentLevel->Start();
}

void sad::Application::PollEvents(bool& isWindowClosed)
{
	InputManager& input = InputManager::GetInstance();
	SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{
        input.UpdateTicks();
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
	rad::RenderBuddy::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_Editor->Clear();

	// Capture the current render in the framebuffer 
	rad::RenderBuddy::BindFrameBuffer();

	// Second 'pass' to recolor outside the framebuffer
	rad::RenderBuddy::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	// Render skybox as cubemap 'behind' scene
	m_Skybox->Draw();

	// Execute threads
	s_ThreadQueue->ExecuteApplicationThreadQueue();

	// Update events subscribed to the update loop
	core::UpdateEvents();

	// Update current level context
	// Rendering and ecs systems
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();
	m_CurrentLevel->Update(world);

	// Update GameCamera
	rad::RenderBuddy::GetCameraInstance()->Update();

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

	m_Editor->Teardown();

	s_MainWindow->Teardown();
}
