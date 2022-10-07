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
    if (GetKeyReleased(SDL_SCANCODE_W))
    {
        spdlog::info("W key released.");
    }

    if (GetKeyPressed(SDL_SCANCODE_E))
    {
        spdlog::info("E key pressed.");
    }

    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        UpdateKeyboardState(event.key.keysym.scancode, CurrentKeyboardStates[event.key.keysym.scancode]);
    }
}

bool sad::InputManager::GetKey(SDL_Scancode key) 
{
    return GetKeyboardState(key);
}

bool sad::InputManager::GetKeyPressed(SDL_Scancode key)
{
    if (!GetKeyboardState(key) && CurrentKeyboardStates[key]) {
        UpdateKeyboardState(key, true);
        return true;
    }
    return false;
}

bool sad::InputManager::GetKeyReleased(SDL_Scancode key)
{
    if (GetKeyboardState(key) && !CurrentKeyboardStates[key]) {
        UpdateKeyboardState(key, false);
        return true;
    }
    return false;
}

bool sad::InputManager::GetMouseButton(SDL_MouseButtonEvent button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonPressed(SDL_MouseButtonEvent button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonReleased(SDL_MouseButtonEvent button)
{
    return false;
}




