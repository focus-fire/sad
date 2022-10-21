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
 * @brief Set the controller when a new controller is connected;
 * @param e 
*/
void sad::InputManager::OnControllerConnected(SDL_ControllerDeviceEvent& e)
{
    if (SDL_IsGameController(e.which))
    {
        if (ControllerIsActive)
        {
            core::Log(ELogType::Error, "Tried to connect a controller but there is already one connected");
            return;
        }

        ControllerIsActive = true;
        controller = SDL_GameControllerOpen(e.which);
        core::Log(ELogType::Info, "Controller connected successfully!");
    }
    else
    {
        core::Log(ELogType::Error, "There was an error connecting controller");
    }
}

void sad::InputManager::OnControllerDisconnected(SDL_ControllerDeviceEvent& e)
{
    core::Log(ELogType::Info, "Controller Disconnected");
    ControllerIsActive = false;
    controller = nullptr;
}

bool sad::InputManager::GetButton(SDL_GameControllerButton button)
{
    if (controller == nullptr)
        return false;

    return SDL_GameControllerGetButton(controller, button);
}

bool sad::InputManager::GetButtonPressed(SDL_GameControllerButton button)
{
    if (controller == nullptr)
        return false;

    if (!GetControllerButtonState(button) && SDL_GameControllerGetButton(controller, button)) {
        UpdateControllerButtonState(button, true);
        return true;
    }
    return false;
}

bool sad::InputManager::GetButtonReleased(SDL_GameControllerButton button)
{
    if (controller == nullptr)
        return false;

    if (GetControllerButtonState(button) && !SDL_GameControllerGetButton(controller, button)) {
        UpdateControllerButtonState(button, false);
        return true;
    }
    return false;
}

float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
    return 0.0f;
}

/**
 * @brief Returns true if the key coresponding to the scancode is down.
 * @param key 
 * @return 
*/
bool sad::InputManager::GetKey(SDL_Scancode key) 
{
    UpdateKeyboardState(key, CurrentKeyboardStates[key]);
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




