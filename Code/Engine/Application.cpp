#include "sadpch.h"

#include "Application.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Sample/Cube.h"

sad::Application::Application()
	: m_Window(new Window())
	, m_Renderer(new sad::rad::Renderer)
	, m_Editor(new cap::Cap())
{ 
	m_Window->Start();
	m_Window->CreateGLContext();
}

sad::Application::~Application()
{
	if (m_Window) 
		delete m_Window;

	if (m_Renderer)
		delete m_Renderer;

	if (m_Editor) 
		delete m_Editor;
}

void sad::Application::Start()
{
	SDL_Window* sdlWindow = m_Window->GetSDLWindow();
	SDL_GLContext glContext = m_Window->GetGLContext();

	// Launch editor alongside Engine
	m_Editor->Start(sdlWindow, glContext);

	// Initialize GL
	GL_CALL(glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight()));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glEnable(GL_DEPTH_TEST));

	// Initialize a vertex buffer with the proper geometry
	sad::rad::VertexArray cubeVa = sad::rad::VertexArray();
	sad::rad::VertexBuffer cubeVb = sad::rad::VertexBuffer(CubePoints, sizeof(CubePoints));

	// Set vertex attributes in attribute container
	sad::rad::VertexAttributeContainer cubeAttributeContainer;
	cubeAttributeContainer.AddFloatAttribute(3); // Positions, Vec3, Attribute Position 0
	cubeAttributeContainer.AddFloatAttribute(2); // Texture Coordinates, Vec2, Attribute Position 1

	// Set vertex array with geometry and attribute data
	cubeVa.AddBufferWithAttributes(cubeVb, cubeAttributeContainer);

	// Create index buffer for cube indices
	sad::rad::IndexBuffer cubeIb = sad::rad::IndexBuffer(CubeIndices, CubeIndexCount);

	// Create view matrices
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), m_Window->GetAspectRatio(), 1.0f, 20.0f);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 vpMatrix = projectionMatrix * viewMatrix;

	// Create Shader
	sad::rad::Shader flatShader = sad::rad::Shader("..\\Data\\Shaders\\Flat.glsl");
	flatShader.Bind();
	flatShader.SetUniform4f("u_Color", 0.85f, 0.85f, 0.85f, 1.0f);

	// Create Texture and bind it to GL_TEXTURE0 or slot 0
	sad::rad::Texture defaultTexture = sad::rad::Texture("..\\Data\\Textures\\Default.png");
	defaultTexture.Bind(1);
	flatShader.SetUniform1i("u_Texture", 1);

	// Create framebuffer 
	sad::rad::FrameBuffer frameBuffer = sad::rad::FrameBuffer(m_Window->GetWidth(), m_Window->GetHeight());
	frameBuffer.Bind();

	// Create empty texture and bind it to the framebuffer
	sad::rad::Texture frameBufferTexture = sad::rad::Texture(m_Window->GetWidth(), m_Window->GetHeight());
	frameBufferTexture.Bind(0);
	frameBufferTexture.AttachToFramebuffer();

	// Create renderbuffer and bind it to the framebuffer
	sad::rad::RenderBuffer renderBuffer = sad::rad::RenderBuffer(m_Window->GetWidth(), m_Window->GetHeight());
	renderBuffer.AttachToFrameBuffer();

	// Clear buffers
	cubeVa.Unbind();
	cubeVb.Unbind();
	cubeIb.Unbind();
	flatShader.Unbind();
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

		/* Draw */
		flatShader.Bind();
		flatShader.SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
		m_Renderer->Draw(cubeVa, cubeIb, flatShader);

		frameBuffer.Unbind();

		/* Editor */
		m_Editor->RenderGameWindow(frameBufferTexture.GetTextureId(), m_Window->GetWidth() / 2, m_Window->GetHeight() / 2);
		m_Editor->Render();

		/* Window */
		m_Window->Render();
	}

	Teardown();
}

void sad::Application::Teardown()
{ 
	// Teardown the engine application here
	m_Editor->Teardown();
	m_Window->Teardown();
}
