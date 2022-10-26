#include "sadpch.h"

#include "InputManager.h"

#include "ConfigManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

sad::InputManager& sad::InputManager::GetInstance()
{
    static sad::InputManager instance;
    return instance;
}

// Keyboard Events

void sad::InputManager::CatchKeyboardEvents(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        keyboardState[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
        keyboardUpdateFrames[event.key.keysym.scancode] = UpdateCounter;
    }
}

bool sad::InputManager::GetKey(SDL_Scancode key) 
{
    return SDL_GetKeyboardState(nullptr)[key];
}

bool sad::InputManager::GetKeyPressed(SDL_Scancode key)
{
    return keyboardState[key] && (keyboardUpdateFrames[key] == UpdateCounter);
}

bool sad::InputManager::GetKeyReleased(SDL_Scancode key)
{
    return !keyboardState[key] && (keyboardUpdateFrames[key] == UpdateCounter);
}

// Mouse Events

void sad::InputManager::CatchMouseEvents(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        mouseState[event.button.button] = (event.type == SDL_MOUSEBUTTONDOWN);
        mouseUpdateFrames[event.button.button] = UpdateCounter;
    }
}

bool sad::InputManager::GetMouseButton(int button)
{
    return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button));
}

bool sad::InputManager::GetMouseButtonPressed(int button)
{
    return mouseState[button] && (mouseUpdateFrames[button] == UpdateCounter);
}

bool sad::InputManager::GetMouseButtonReleased(int button)
{
    return !mouseState[button] && (mouseUpdateFrames[button] == UpdateCounter);
}

// Controller Events

void sad::InputManager::OnControllerConnected(SDL_ControllerDeviceEvent& event)
{
    if (SDL_IsGameController(event.which))
    {
        if (ControllerIsActive)
        {
            core::Log(ELogType::Error, "Tried to connect a controller but there is already one connected");
            return;
        }

        ControllerIsActive = true;
        controller = SDL_GameControllerOpen(event.which);

        core::Log(ELogType::Trace, "Initializing Control Settings");
        ControllerDeadZone = std::stof(sad::ConfigManager::GetValue("controls", "deadzone"));

        core::Log(ELogType::Info, "Controller connected successfully!");
    }
    else
    {
        core::Log(ELogType::Error, "There was an error connecting controller");
    }
}

void sad::InputManager::OnControllerDisconnected()
{
    core::Log(ELogType::Info, "Controller Disconnected");
    ControllerIsActive = false;
    controller = nullptr;
}

void sad::InputManager::CatchControllerEvents(SDL_Event& event)
{
    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
    {
        buttonState[event.cbutton.button] = (event.type == SDL_CONTROLLERBUTTONDOWN);
        buttonUpdateFrames[event.cbutton.button] = UpdateCounter;
    }
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

    return buttonState[button] && (buttonUpdateFrames[button] == UpdateCounter);
}

bool sad::InputManager::GetButtonReleased(SDL_GameControllerButton button)
{
    if (controller == nullptr)
        return false;

    return !buttonState[button] && (buttonUpdateFrames[button] == UpdateCounter);
}

float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
    if (controller == nullptr)
        return 0.f;

    float roundedAxis = SDL_GameControllerGetAxis(controller, axis) / 32767.f;
    roundedAxis = std::ceil(roundedAxis * 10.0) / 10.0;

    return roundedAxis;
}