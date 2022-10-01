#include "Application.h"
#include "InputManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::Application::Application()
{
    SDL_Init(SDL_INIT_VIDEO);
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

	while (!isClosed) 
	{
		InputManager::GetInstance().Update();
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
				isClosed = true;
		}
	}

	End();
}

void sad::Application::End()
{
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}
