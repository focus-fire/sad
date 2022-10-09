#include "sadpch.h"

#include "Application.h"

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <SDL2/SDL.h>

#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "ECS/RenderableObjectSystem.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/RenderBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"
#include "Renderer/Sample/Cube.h"

#include "RenderableResource.h"
#include "RenderableObject.h"

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
	SDL_Window* sdlWindow = s_MainWindow->GetSDLWindow();
	SDL_GLContext glContext = s_MainWindow->GetGLContext();

	// Create sample resource for a cube
	RenderableResource::Geometry cubeGeometry { CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount };
	RenderableResource cubeResource = RenderableResource(&cubeGeometry);

	// Get entity world and create entity
	sad::ecs::EntityWorld* world = sad::ecs::Registry::GetEntityWorld();
	sad::ecs::Entity cubeEntity = sad::ecs::Entity();
	cubeEntity.AddComponent<RenderableResourceComponent>({ &cubeResource });

	// Launch editor alongside Engine
	m_Editor->Start(sdlWindow, glContext);

	// Initialize GL
	GL_CALL(glViewport(0, 0, s_MainWindow->GetWidth(), s_MainWindow->GetHeight()));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glEnable(GL_DEPTH_TEST));

	// Create view matrices
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 vpMatrix = projectionMatrix * viewMatrix;

	// Create framebuffer 
	sad::rad::FrameBuffer frameBuffer = sad::rad::FrameBuffer(s_MainWindow->GetWidth(), s_MainWindow->GetHeight());
	frameBuffer.Bind();

	// Create empty texture and bind it to the framebuffer
	sad::rad::Texture frameBufferTexture = sad::rad::Texture(s_MainWindow->GetWidth(), s_MainWindow->GetHeight());
	frameBufferTexture.Bind(0);
	frameBufferTexture.AttachToFramebuffer();

	// Create renderbuffer and bind it to the framebuffer
	sad::rad::RenderBuffer renderBuffer = sad::rad::RenderBuffer(s_MainWindow->GetWidth(), s_MainWindow->GetHeight());
	renderBuffer.AttachToFrameBuffer();

	// Clear buffers
	frameBuffer.Unbind();

	// Rotation Logic
	float rotAngle = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

	bool isClosed = false;
	SDL_Event event;

	while (!isClosed) 
	{
		while (SDL_PollEvent(&event)) 
		{
			m_Editor->CatchSDLEvents(event);

			if (event.type == SDL_QUIT) 
				isClosed = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdlWindow))
				isClosed = true;
		}

		// First 'pass' sets up the framebuffer
		// This clear color is the background for the game
		m_Renderer->Clear(0.55f, 0.65f, 0.50f, 1.0f);
		m_Editor->Clear();
		
		// Capture the current render in the framebuffer 
		frameBuffer.Bind();

		// Second 'pass' to recolor outside the framebuffer
		m_Renderer->Clear(0.45f, 0.55f, 0.60f, 1.0f);

		/* Update */
		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		rotAngle += 0.001f * elapsedTime;
		if (rotAngle >= 360.0f)
			rotAngle = 0.0f;

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 mvpMatrix = vpMatrix * modelMatrix;

		/* Update ECS Systems */
		sad::ecs::RenderableObjectSystem::Update();

		/* Draw */
		auto view = world->view<const RenderableObjectComponent>();
		for (auto [entity, obj] : view.each())
		{
			RenderableObjectComponent renderable = obj;

			sad::rad::VertexArray* va = renderable.m_RenderableObject->GetVertexArray();
			sad::rad::IndexBuffer* ib = renderable.m_RenderableObject->GetIndexBuffer();
			sad::rad::Shader* shader = renderable.m_RenderableObject->GetShader();
			shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

			m_Renderer->Draw(va, ib, shader);
		}

		// Unbind framebuffer for next pass
		frameBuffer.Unbind();

		/* Editor */
		m_Editor->RenderGameWindow(frameBufferTexture.GetTextureId());
		m_Editor->Render();

		/* Window */
		s_MainWindow->Render();

		/* Events */
		core::UpdateEvents();
	}

	Teardown();
}

void sad::Application::Teardown()
{ 
	m_Editor->Teardown();
	s_MainWindow->Teardown();
}
