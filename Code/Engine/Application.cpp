#include "sadpch.h"

#include "Application.h"
#include "InputManager.h"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Game/Time.h>

#include "ECS/Registry.h"
#include "ECS/Systems/RenderableObjectSystem.h"
#include "ECS/Components/RenderableResourceComponent.h"
#include "ECS/Components/RenderableObjectComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PlayerControllerComponent.h"

#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Sample/Cube.h"

#include "Transform.h"
#include "RenderableObject.h"
#include "PlayerController.h"

sad::Window* sad::Application::s_MainWindow;

sad::Application::Application()
{ 
	s_MainWindow = new sad::Window;
	s_MainWindow->Start();
	s_MainWindow->CreateGLContext();

	// Renderer and Editor have to be initialized after the main window
	m_Renderer = new sad::rad::Renderer;
	m_Editor = new cap::Editor;
}

sad::Application::~Application()
{
	delete m_Renderer;
	delete s_MainWindow;
	delete m_Editor;

	// Remove when resource is abstracted
	delete m_CubeResource;
}

void sad::Application::EngineStart()
{
	// Launch editor 
	m_Editor->Start();

	// Initialize the renderer and save a pointer to the FrameBuffer for the editor
	m_Renderer->Start();

	// Create sample resource for a cube
	RenderableResource::Geometry cubeGeometry { CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount };
	m_CubeResource = new RenderableResource(cubeGeometry);

	// Add resource and transform components to the entities
	m_FirstCubeEntity.AddComponent<ecs::RenderableResourceComponent>({ m_CubeResource });
	m_FirstCubeEntity.AddComponent<ecs::TransformComponent>({ &m_FirstCubeEntity.Transform });
	m_FirstCubeEntity.AddEmptyComponent<ecs::PlayerControllerComponent>({});

	m_SecondCubeEntity.AddComponent<ecs::RenderableResourceComponent>({ m_CubeResource });
	m_SecondCubeEntity.AddComponent<ecs::TransformComponent>({ &m_SecondCubeEntity.Transform });

	// Create view matrices 
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f), // Camera position
		glm::vec3(0.0f, -0.5f, 0.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);
	m_VpMatrix = projectionMatrix * viewMatrix;

	// Translation Logic (-pi to pi for demo)
	m_CubeTranslate = -1.0f * glm::pi<float>();
	m_LastTime = std::chrono::steady_clock::now();

	bool isClosed = false;

	// Sample Event Signal For "UI" Group - Can Delete
	core::SignalEvent("UI");
  
	// Game Start
	this->Start();

	while (!isClosed) 
	{
		PollEvents(&isClosed);
		
		float dt = pog::Time::GetDeltaTime();

		// Game Update
		this->Update(dt);

		// Engine Update
		sad::Application::Update(dt);
	}

	Teardown();
}

void sad::Application::PollEvents(bool* isClosed)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{
		m_Editor->CatchSDLEvents(event);

		if (event.type == SDL_QUIT) 
			*isClosed = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_MainWindow->GetSDLWindow()))
			*isClosed = true;
		if (event.type == SDL_CONTROLLERDEVICEADDED)
			InputManager::GetInstance().OnControllerConnected(event.cdevice);
		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
			InputManager::GetInstance().OnControllerDisconnected(event.cdevice);
	}
}

void sad::Application::Update(float dt)
{
	// First 'pass' sets up the framebuffer
	// This clear color is the background for the game
	m_Renderer->Clear(0.55f, 0.65f, 0.50f, 1.0f);
	m_Editor->Clear();

	// Capture the current render in the framebuffer 
	m_Renderer->BindFrameBuffer();

	// Second 'pass' to recolor outside the framebuffer
	m_Renderer->Clear(0.45f, 0.55f, 0.60f, 1.0f);

	/* Update Game Logic */

	// TODO: Prevent updates in game if in Editor mode 
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	m_CubeTranslate += 1.0f * dt;
	if (m_CubeTranslate >= glm::pi<float>())
		m_CubeTranslate = -1.0f * glm::pi<float>();

	// Manipulate first entity transform
	m_FirstCubeEntity.Transform.Rotate(glm::vec3(10.0f * dt));
	m_FirstCubeEntity.Transform.Translate(glm::vec3(0.0f, glm::sin(m_CubeTranslate) * dt, 0.0f));

	// Manipulate second entity transform
	m_SecondCubeEntity.Transform.Rotate(glm::vec3(10.0f * dt));
	m_SecondCubeEntity.Transform.Translate(glm::vec3(glm::sin(-m_CubeTranslate * 2) * dt, 0.0f, 0.0f));

	/* Update ECS Systems */
	core::UpdateEvents();
	ecs::RenderableObjectSystem::Update();
	PlayerController::Update();

	/* Draw */
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// TODO: Move render queries into a RenderingSystem
	auto view = world.view<const ecs::RenderableObjectComponent, const ecs::TransformComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent] : view.each())
	{
		ecs::RenderableObjectComponent renderable = renderableObjectComponent;

		rad::VertexArray* va = renderable.m_RenderableObject->GetVertexArray();
		rad::IndexBuffer* ib = renderable.m_RenderableObject->GetIndexBuffer();
		rad::Shader* shader = renderable.m_RenderableObject->GetShader();

		// TODO: Retrieve the view projection matrix from the Camera 
		glm::mat4 mvpMatrix = m_VpMatrix * transformComponent.m_Transform->GetTransformMatrix();
		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

		m_Renderer->Draw(va, ib, shader);
	}

	// Unbind framebuffer for next pass
	m_Renderer->UnbindFrameBuffer();

	/* Editor */
	m_Editor->Render(m_Renderer->GetFrameBufferTexture());

	/* Window */
	s_MainWindow->Render();
}

void sad::Application::Teardown()
{ 
	m_Editor->Teardown();
	s_MainWindow->Teardown();
}
