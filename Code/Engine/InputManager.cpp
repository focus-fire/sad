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
        m_keyboardState[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
        m_keyboardUpdateFrames[event.key.keysym.scancode] = UpdateCounter;
    }
}

bool sad::InputManager::GetKey(SDL_Scancode key) 
{
    return SDL_GetKeyboardState(nullptr)[key];
}

bool sad::InputManager::GetKeyPressed(SDL_Scancode key)
{
    return m_keyboardState[key] && (m_keyboardUpdateFrames[key] == UpdateCounter);
}

bool sad::InputManager::GetKeyReleased(SDL_Scancode key)
{
    return !m_keyboardState[key] && (m_keyboardUpdateFrames[key] == UpdateCounter);
}

// Mouse Events

void sad::InputManager::CatchMouseEvents(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_mouseState[event.button.button] = (event.type == SDL_MOUSEBUTTONDOWN);
        m_mouseUpdateFrames[event.button.button] = UpdateCounter;
    }
}

bool sad::InputManager::GetMouseButton(int button)
{
    return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button));
}

bool sad::InputManager::GetMouseButtonPressed(int button)
{
    return m_mouseState[button] && (m_mouseUpdateFrames[button] == UpdateCounter);
}

bool sad::InputManager::GetMouseButtonReleased(int button)
{
    return !m_mouseState[button] && (m_mouseUpdateFrames[button] == UpdateCounter);
}

// Controller Events

void sad::InputManager::OnControllerConnected(SDL_ControllerDeviceEvent& event)
{
    if (SDL_IsGameController(event.which))
    {
        if (m_controllerIsActive)
        {
            core::Log(ELogType::Error, "Tried to connect a controller but there is already one connected");
            return;
        }

        m_controllerIsActive = true;
        m_controller = SDL_GameControllerOpen(event.which);

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
    m_controllerIsActive = false;
    m_controller = nullptr;
}

void sad::InputManager::CatchControllerEvents(SDL_Event& event)
{
    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
    {
        m_buttonState[event.cbutton.button] = (event.type == SDL_CONTROLLERBUTTONDOWN);
        m_buttonUpdateFrames[event.cbutton.button] = UpdateCounter;
    }
}

bool sad::InputManager::GetButton(SDL_GameControllerButton button)
{
    if (m_controller == nullptr)
        return false;

    return SDL_GameControllerGetButton(m_controller, button);
}

bool sad::InputManager::GetButtonPressed(SDL_GameControllerButton button)
{
    if (m_controller == nullptr)
        return false;

    return m_buttonState[button] && (m_buttonUpdateFrames[button] == UpdateCounter);
}

bool sad::InputManager::GetButtonReleased(SDL_GameControllerButton button)
{
    if (m_controller == nullptr)
        return false;

    return !m_buttonState[button] && (m_buttonUpdateFrames[button] == UpdateCounter);
}

float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
    if (m_controller == nullptr)
        return 0.f;

    float roundedAxis = SDL_GameControllerGetAxis(m_controller, axis) / 32767.f;
    roundedAxis = static_cast<float>(std::ceil(roundedAxis * 10.0) / 10.0);

    return roundedAxis;
}