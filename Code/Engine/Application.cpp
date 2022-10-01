#include "Application.h"
#include "InputManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::Application::Application()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
    m_Window = SDL_CreateWindow("this engine is sad.", 150, 150, 800, 600, SDL_WINDOW_SHOWN);
}

sad::Application::~Application()
{
	// Destroy the engine here... 
}

void sad::Application::Start()
{
	bool isClosed = false;
	SDL_Event event;
	SDL_Joystick* joy = SDL_JoystickOpen(0);

	while (!isClosed) 
	{	
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
				isClosed = true;
			InputManager::GetInstance().CatchKeyboardEvent(event);
			InputManager::GetInstance().CatchGamepadEvent(event, joy);
		}
	}

	End();
}

void sad::Application::End()
{
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
