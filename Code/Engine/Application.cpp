#include "Application.h"

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

sad::Application::Application()
{
    // Initialize the application here...

    // Temporary SDL code for testing...
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Test", 150, 150, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();
}

sad::Application::~Application()
{
    // Destroy the engine here...
}