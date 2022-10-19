#include "sadpch.h"

#include "InputManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::InputManager& sad::InputManager::GetInstance()
{
    static sad::InputManager instance;
    return instance;
}

/**
 * @brief Catches joystick and button input from controller
 * @param event sdl event
 * @param joy sdl joystick
*/
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
                    core::Log(ELogType::Info, "JOYSTICK LEFT");
                }
                //Right of dead zone
                else if (event.jaxis.value > 8000)
                {
                    core::Log(ELogType::Info, "JOYSTICK RIGHT");
                }
            }

            //Y axis motion
            else if (event.jaxis.axis == 1)
            {
                //Below of dead zone
                if (event.jaxis.value < -8000)
                {
                    core::Log(ELogType::Info, "JOYSTICK DOWN");
                }
                //Above of dead zone
                else if (event.jaxis.value > 8000)
                {
                    core::Log(ELogType::Info, "JOYSTICK UP");
                }
            }
        }
    }
}

/**
 * @brief Catches keyboard events from SDL_Event and updates the keyboard state;
 * @param event 
*/
void sad::InputManager::CatchKeyboardEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        UpdateKeyboardState(event.key.keysym.scancode, CurrentKeyboardStates[event.key.keysym.scancode]);
    }
}

/**
 * @brief Returns true if the key coresponding to the scancode is down.
 * @param key 
 * @return 
*/
bool sad::InputManager::GetKey(SDL_Scancode key) 
{
    return GetKeyboardState(key);
}

/**
 * @brief Returns true if key coresponding to the scancode is pressed before the last frame.
 * @param key 
 * @return 
*/
bool sad::InputManager::GetKeyPressed(SDL_Scancode key)
{
    if (!GetKeyboardState(key) && CurrentKeyboardStates[key]) {
        UpdateKeyboardState(key, true);
        return true;
    }
    return false;
}

/**
 * @brief Returns true if key coresponding to the scancode is released before the last frame.
 * @param key
 * @return
*/
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




