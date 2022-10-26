#include "sadpch.h"

#include "Application.h"
#include "InputManager.h"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <glad/glad.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "ECS/Registry.h"
#include "ECS/Entity.h"
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
#include "RenderableResource.h"
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
	delete s_MainWindow;
	delete m_Renderer;
	delete m_Editor;
}

void sad::Application::Start()
{
	// Launch editor 
	m_Editor->Start();

	// Initialize the renderer and save a pointer to the FrameBuffer for the editor
	m_Renderer->Start();

	// Create sample resource for a cube
	RenderableResource::Geometry cubeGeometry { CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount };
	RenderableResource cubeResource = RenderableResource(cubeGeometry);

	// Get entity world and create entity
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	sad::ecs::Entity cubeEntity = sad::ecs::Entity();
	sad::ecs::Entity secondCubeEntity = sad::ecs::Entity();

	PlayerController controller = PlayerController();

	// Add resource and transform components to the entities
	cubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ &cubeResource });
	cubeEntity.AddComponent<sad::ecs::TransformComponent>({ &cubeEntity.Transform });
	cubeEntity.AddEmptyComponent<sad::ecs::PlayerControllerComponent>({});

	secondCubeEntity.AddComponent<sad::ecs::RenderableResourceComponent>({ &cubeResource });
	secondCubeEntity.AddComponent<sad::ecs::TransformComponent>({ &secondCubeEntity.Transform });

	// Create view matrices 
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f), // Camera position
		glm::vec3(0.0f, -0.5f, 0.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);
	glm::mat4 vpMatrix = projectionMatrix * viewMatrix;

	// Translation Logic (-pi to pi for demo)
	float translate = -1.0f * glm::pi<float>();
	std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

	bool isClosed = false;
	SDL_Event event;
	InputManager& input = InputManager::GetInstance();

	while (!isClosed) 
	{	
		input.IncrementUpdateCounter();

		while (SDL_PollEvent(&event)) 
		{
			m_Editor->CatchSDLEvents(event);
			input.CatchMouseEvents(event);
			input.CatchKeyboardEvents(event);
			input.CatchControllerEvents(event);

			if (event.type == SDL_QUIT) 
				isClosed = true;

			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_MainWindow->GetSDLWindow()))
				isClosed = true;

			if (event.type == SDL_CONTROLLERDEVICEADDED)
				input.OnControllerConnected(event.cdevice);

			if (event.type == SDL_CONTROLLERDEVICEREMOVED)
				input.OnControllerDisconnected(event.cdevice);

			if (event.type == SDL_MOUSEMOTION) 
				input.SetMousePosition(event.motion.x, event.motion.y);
			
		}

		// First 'pass' sets up the framebuffer
		// This clear color is the background for the game
		m_Renderer->Clear(0.55f, 0.65f, 0.50f, 1.0f);
		m_Editor->Clear();

		// Capture the current render in the framebuffer 
		m_Renderer->BindFrameBuffer();

		// Second 'pass' to recolor outside the framebuffer
		m_Renderer->Clear(0.45f, 0.55f, 0.60f, 1.0f);

		/* Update */

		/* Update Game Logic */
		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		translate += 0.001f * elapsedTime;
		if (translate >= glm::pi<float>())
			translate = -1.0f * glm::pi<float>();

		//// Manipulate second entity transform
		secondCubeEntity.Transform.Rotate(glm::vec3(1.0f * elapsedTime / 50.0f));
		secondCubeEntity.Transform.Translate(glm::vec3(glm::sin(translate) / 100.0f, 0.0f, 0.0f));
		secondCubeEntity.Transform.SetScale(glm::vec3(1.0f));


		/* Update ECS Systems */
		sad::ecs::RenderableObjectSystem::Update();
		PlayerController::Update();

		/* Draw */
		auto view = world.view<const sad::ecs::RenderableObjectComponent, const sad::ecs::TransformComponent>();
		for (auto [entity, renderableObjectComponent, transformComponent] : view.each())
		{
			sad::ecs::RenderableObjectComponent renderable = renderableObjectComponent;

			sad::rad::VertexArray* va = renderable.m_RenderableObject->GetVertexArray();
			sad::rad::IndexBuffer* ib = renderable.m_RenderableObject->GetIndexBuffer();
			sad::rad::Shader* shader = renderable.m_RenderableObject->GetShader();

			// TODO: Retrieve the view projection matrix from the Camera 
			glm::mat4 mvpMatrix = vpMatrix * transformComponent.m_Transform->GetTransformMatrix();
			shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

			m_Renderer->Draw(va, ib, shader);
		}
		

		// Unbind framebuffer for next pass
		m_Renderer->UnbindFrameBuffer();

		/* Editor */
		m_Editor->RenderGameWindow(m_Renderer->GetFrameBufferTexture());
		m_Editor->Render();

		/* Window */
		s_MainWindow->Render();
	}

	Teardown();
}

void sad::Application::Teardown()
{ 
	m_Editor->Teardown();
	s_MainWindow->Teardown();
}
