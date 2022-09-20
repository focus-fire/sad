#include "Application.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::Game::Application::Application()
	: sad::Application()
{
	// Game initializes here

    // Temporary SDL code for testing...
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("this engine is sad.", 150, 150, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Delay(5000);

	if (!window) 
		spdlog::error("failed to created window: %s", SDL_GetError());

	bool isClosed = false;
	SDL_Event event;

	while (!isClosed) 
	{
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
				isClosed = true;
		}
	}

    SDL_DestroyWindow(window);
    SDL_Quit();
}

sad::Game::Application::~Application()
{
	// Game is destroyed here
}

