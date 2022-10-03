#include "InputManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::InputManager& sad::InputManager::GetInstance()
{
    static sad::InputManager instance;
    return instance;
}

void sad::InputManager::CatchGamepadEvent(SDL_Event& event, SDL_Joystick *joy)
{
    if (event.type == SDL_JOYAXISMOTION)
    {
        if (event.jaxis.which == 0) {
            //X axis motion
            if (event.jaxis.axis == 0)
            {
                //Left of dead zone
                if (event.jaxis.value < -8000)
                {
                    spdlog::info("CONTROLLER LEFT");
                }
                //Right of dead zone
                else if (event.jaxis.value > 8000)
                {
                    spdlog::info("CONTROLLER RIGHT");
                }
            }
            //Y axis motion
            else if (event.jaxis.axis == 1)
            {
                //Below of dead zone
                if (event.jaxis.value < -8000)
                {
                    spdlog::info("CONTROLLER DOWN");
                }
                //Above of dead zone
                else if (event.jaxis.value > 8000)
                {
                    spdlog::info("CONTROLLER UP");
                }
            }
        }
    }
}

void sad::InputManager::CatchKeyboardEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            if (!KeyUp)
            {
                // Notify forward key pressed 

            }
            KeyUp = true;
            spdlog::info("FORWARD key pressed.");
            break;

        case SDLK_s:
            if (!KeyDown)
            {
                // Notify backward key pressed 

            }
            KeyDown = true;
            spdlog::info("BACKWARD key pressed.");
            break;

        case SDLK_d:
            if (!KeyRight)
            {
                // Notify right key pressed 

            }
            KeyRight = true;
            spdlog::info("RIGHT key pressed.");
            break;

        case SDLK_a:
            if (!KeyLeft)
            {
                // Notify left key pressed

            }
            KeyLeft = true;
            spdlog::info("LEFT key pressed.");
            break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            if (KeyUp)
            {
                // Notify forward key released 
            }
            KeyUp = false;
            break;

        case SDLK_s:
            if (KeyDown)
            {
                // Notify backward key released 
            }
            KeyDown = false;
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

bool sad::InputManager::GetKey(Direction key)
{
    return GetKeyState(key);
}

bool sad::InputManager::GetKeyPressed(Direction key)
{
    return false;
}

bool sad::InputManager::GetKeyReleased(Direction key)
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




