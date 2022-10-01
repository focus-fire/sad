#include "InputManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::InputManager sad::InputManager::s_Instance;

sad::InputManager& sad::InputManager::GetInstance()
{
    return s_Instance;
}

sad::InputManager::InputManager() {}

sad::InputManager::~InputManager() {}

void sad::InputManager::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN) 
        {
            switch (event.key.keysym.sym) 
            {
            case SDLK_w:
                if (!KeyForward) 
                {
                    // Notify forward key pressed 
                    spdlog::info("FORWARD key pressed.");
                }
                KeyForward = true;
                break;

            case SDLK_s:
                if (!KeyBackward)
                {
                    // Notify backward key pressed 
                    spdlog::info("BACKWARD key pressed.");
                }
                KeyBackward = true;
                break;

            case SDLK_d:
                if (!KeyRight)
                {
                    // Notify right key pressed 
                    spdlog::info("RIGHT key pressed.");
                }
                KeyRight = true;
                break;

            case SDLK_a:
                if (!KeyLeft)
                {
                    // Notify left key pressed
                    spdlog::info("LEFT key pressed.");
                }
                KeyLeft = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                if (KeyForward)
                {
                    // Notify forward key released 
                }
                KeyForward = false;
                break;

            case SDLK_s:
                if (KeyBackward)
                {
                    // Notify backward key released 
                }
                KeyBackward = false;
                break;

            case SDLK_d:
                if (KeyRight)
                {
                    // Notify right key released 
                }
                KeyRight = false;
                break;

            case SDLK_a:
                if (KeyLeft)
                {
                    // Notify left key released
                }
                KeyLeft = false;
                break;
            }
        }
    }
}

bool sad::InputManager::GetKey(int key)
{
    return false;
}

bool sad::InputManager::GetKeyPressed(int key)
{
    return false;
}

bool sad::InputManager::GetKeyReleased(int key)
{
    return false;
}

bool sad::InputManager::GetMouseButton(int button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonPressed(int button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonReleased(int button)
{
    return false;
}




