#include "sadpch.h"

#include <glad/glad.h>

#include "Window.h"

#include "ConfigManager.h"

#include "AudioManager.h"

sad::Window::Window(const WindowProperties& properties)
	: m_Properties(properties)
{
	core::Log(ELogType::Info, "[Window] Creating a window [{} - {}x{}]", m_Properties.Title, m_Properties.Width, m_Properties.Height);

	int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	SAD_ASSERT(result >= 0, "Failed to initialize SDL context");

	// OpenGL Attribute Sets for SDL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef _SAD_WINDOWS
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#elif _SAD_MAC
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
}

void sad::Window::Start()
{
	SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_Window = SDL_CreateWindow(m_Properties.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Properties.Width, m_Properties.Height, flags);
	SAD_ASSERT(m_Window != nullptr, "Failed to create SDL window");

	int mixerInitialization = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);
	SAD_ASSERT(mixerInitialization >= 0, "Failed to open audio device for SDL_Mixer");

	SDL_SetWindowMaximumSize(m_Window, m_Properties.MaxDimensions.first, m_Properties.MaxDimensions.second);
	SDL_SetWindowMinimumSize(m_Window, m_Properties.MinDimensions.first, m_Properties.MinDimensions.second);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
}

void sad::Window::CreateGLContext()
{
	m_GLContext = SDL_GL_CreateContext(m_Window);
	SAD_ASSERT(m_GLContext, "Failed to initialize GL context");
	
	// Mandatory to setup and trigger event in order to enable GL_CALL
	core::InitializeListener("OnGLStateChanged", OnGLStateChanged);
	core::SignalEvent("OnGLStateChanged");

	int vsync = SDL_GL_SetSwapInterval(std::stoi(ConfigManager::GetValue("graphics", "vsync"))); // Enable VSync
	SAD_ASSERT(vsync >= 0, "Failed to enable VSync");

	int glad = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	SAD_ASSERT(glad, "Failed to initialize GLAD");

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	core::Log(ELogType::Info, "Aha! Welcome to OpenGL v{}", version);
}

void sad::Window::Render()
{
	SDL_GL_SwapWindow(m_Window);
}

void sad::Window::Teardown()
{
    SDL_DestroyWindow(m_Window);
    SDL_Quit();

	// Mandatory to trigger event in order to disable GL_CALL
	core::SignalEvent("OnGLStateChanged");
}
