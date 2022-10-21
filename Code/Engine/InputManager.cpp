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

/**
 * @brief Unsets the controller when controller disconnected;
 * @param e
*/
void sad::InputManager::OnControllerDisconnected(SDL_ControllerDeviceEvent& e)
{
    core::Log(ELogType::Info, "Controller Disconnected");
    ControllerIsActive = false;
    controller = nullptr;
}

/**
 * @brief Returns true if a specified controller button is held.
 * @param button
 * @return
*/
bool sad::InputManager::GetButton(SDL_GameControllerButton button)
{
    if (controller == nullptr)
        return false;

    return SDL_GameControllerGetButton(controller, button);
}

/**
 * @brief Returns true if a specified controller button is pressed in the current frame.
 * @param button
 * @return
*/
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

/**
 * @brief Returns true if a specified controller button is released in the current frame.
 * @param button
 * @return
*/
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

/**
 * @brief Returns the value rounded to 1.0 of a given axis
 * @param axis 
 * @return 
*/
float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
    if (controller == nullptr)
        return 0.f;

    float roundedAxis = SDL_GameControllerGetAxis(controller, axis) / 32767.f;
    roundedAxis = std::ceil(roundedAxis * 10.0) / 10.0;

    return roundedAxis;
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
 * @brief Returns true if key coresponding to the scancode is pressed in the current frame.
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
 * @brief Returns true if key coresponding to the scancode is released in the current frame.
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




