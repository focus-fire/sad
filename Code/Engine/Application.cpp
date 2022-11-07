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

#include "ECS/Registry.h"
#include "ECS/Systems/RenderableObjectSystem.h"
#include "ECS/Systems/RenderingSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"

#include "Renderer/RenderBuddy.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/ShaderResource.h"
#include "Renderer/Sample/Cube.h"

#include "InputManager.h"
#include "Transform.h"
#include "RenderableObject.h"
#include "EngineStateManager.h"

sad::Window* sad::Application::s_MainWindow;

sad::Application::Application()
{ 
	s_MainWindow = new sad::Window;
	s_MainWindow->Start();
	s_MainWindow->CreateGLContext();

	m_Editor = new cap::Editor;
}

sad::Application::~Application()
{
	delete s_MainWindow;
	delete m_Editor;
}

void sad::Application::EngineStart()
{
	// Launch editor 
	m_Editor->Start();

	// Initialize the renderer and save a pointer to the FrameBuffer for the editor
	rad::RenderBuddy::Start();

	// Import Resources
	ResourceManager::Import();

	// Sample Event Signal For "UI" Group - Can Delete
	core::SignalEvent("UI");
  
	// Game Start
	this->Start();

	bool isClosed = false;
	while (!isClosed) 
	{
		PollEvents(&isClosed);
		
		float dt = pog::Time::GetDeltaTime();

		if (m_EngineStateManager.GetEngineMode() == EEngineMode::Game)
		{
			// Game Update
			this->Update(dt);
		}

		// Engine Update
		sad::Application::Update(dt);
	}

	Teardown();
}

void sad::Application::PollEvents(bool* isClosed)
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
			*isClosed = true;

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_MainWindow->GetSDLWindow()))
			*isClosed = true;

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
	rad::RenderBuddy::ClearColor(glm::vec4(0.55f, 0.65f, 0.50f, 1.0f));
	m_Editor->Clear();

	// Capture the current render in the framebuffer 
	rad::RenderBuddy::BindFrameBuffer();

	// Second 'pass' to recolor outside the framebuffer
	rad::RenderBuddy::ClearColor(glm::vec4(0.45f, 0.55f, 0.60f, 1.0f));

	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// Update ECS systems
	ecs::RenderableObjectSystem::Update(world);
	ecs::PlayerControllerSystem::Update(world);

	// Update events subscribed to the update loop
	core::UpdateEvents();

	// Drawing 
	ecs::RenderingSystem::Draw(world);

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
	m_Editor->Teardown();
	s_MainWindow->Teardown();
}

glm::mat4 sad::Application::GetViewProjectionMatrix()
{
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f), // Camera position
		glm::vec3(0.0f, -0.5f, 0.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);

	return projectionMatrix * viewMatrix;
}
