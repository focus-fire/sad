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

Uint64 counter = 0;
void sad::InputManager::UpdateTicks()
{
    counter += 1;
}

// Keyboard Events

void sad::InputManager::CatchKeyboardEvents(SDL_Event& event)
{
    if (IsInputLocked())
        return;

    if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat)
    {
        m_ControllerBeingUsed = false;
        m_KeyboardState[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
        m_KeyboardUpdateFrames[event.key.keysym.scancode] = counter;
    }
}

bool sad::InputManager::GetKey(KeyCode key)
{
    if (IsInputLocked())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    return SDL_GetKeyboardState(nullptr)[keyScancode];
}

bool sad::InputManager::GetKeyPressed(KeyCode key)
{
    if (IsInputLocked())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    if (m_KeyboardState[keyScancode] && (m_KeyboardUpdateFrames[keyScancode]+1 == counter))
    {
        m_KeyboardUpdateFrames[keyScancode]--;
        return true;
    }

    return false;
}

bool sad::InputManager::GetKeyReleased(KeyCode key)
{
    if (IsInputLocked())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    if (!m_KeyboardState[keyScancode] && (m_KeyboardUpdateFrames[keyScancode] == counter))
    {
        m_KeyboardUpdateFrames[keyScancode]--;
        return true;
    }
    
    return false;
}

// Mouse Events

void sad::InputManager::CatchMouseEvents(SDL_Event& event)
{
    if (IsInputLocked())
        return;

    if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_ControllerBeingUsed = false;
        m_MouseState[event.button.button] = (event.type == SDL_MOUSEBUTTONDOWN);
        m_MouseUpdateFrames[event.button.button] = counter;
    }
}

bool sad::InputManager::GetMouseButton(int button)
{
    if (IsInputLocked())
        return false;

    return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button));
}

bool sad::InputManager::GetMouseButtonPressed(int button)
{
    if (IsInputLocked())
        return false;

    if (m_MouseState[button] && (m_MouseUpdateFrames[button]+1 == counter))
    {
        m_MouseUpdateFrames[button]--;
        return true;
    }
    return false;
}

bool sad::InputManager::GetMouseButtonReleased(int button)
{
    if (IsInputLocked())
        return false;

    if (!m_MouseState[button] && (m_MouseUpdateFrames[button] == counter))
    {
        m_MouseUpdateFrames[button]--;
        return true;
    }
    return false;
}

// Controller Events

void sad::InputManager::OnControllerConnected(SDL_ControllerDeviceEvent& event)
{
    if (SDL_IsGameController(event.which))
    {
        if (m_ControllerIsActive)
        {
            core::Log(ELogType::Error, "Tried to connect a controller but there is already one connected");
            return;
        }

        m_ControllerIsActive = true;
        m_Controller = SDL_GameControllerOpen(event.which);

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
    m_ControllerIsActive = false;
    m_Controller = nullptr;
}

void sad::InputManager::CatchControllerEvents(SDL_Event& event)
{
    if (IsInputLocked())
        return;

    if (GetLeftAxis("Horizontal") > ControllerDeadZone || GetLeftAxis("Vertical") > ControllerDeadZone
        || GetRightAxis("Horizontal") > ControllerDeadZone || GetRightAxis("Vertical") > ControllerDeadZone)
    {
        m_ControllerBeingUsed = true;
    }

    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
    {
        m_ControllerBeingUsed = true;
        m_ButtonState[event.cbutton.button] = (event.type == SDL_CONTROLLERBUTTONDOWN);
        m_ButtonUpdateFrames[event.cbutton.button] = counter;
    }
}

bool sad::InputManager::GetButton(ControllerButton button)
{
    if (IsInputLocked())
        return false;

    if (m_Controller == nullptr)
        return false;

    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    return SDL_GameControllerGetButton(m_Controller, ctrlButton);
}

bool sad::InputManager::GetButtonPressed(ControllerButton button)
{
    if (IsInputLocked())
        return false;

    if (m_Controller == nullptr)
        return false;

    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    if (m_ButtonState[ctrlButton] && (m_ButtonUpdateFrames[ctrlButton]+1 == counter))
    {
        m_ButtonUpdateFrames[ctrlButton]--;
        return true;
    }
    return false;
}

bool sad::InputManager::GetButtonReleased(ControllerButton button)
{
    if (IsInputLocked())
        return false;

    if (m_Controller == nullptr)
        return false;
    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    if (!m_ButtonState[ctrlButton] && (m_ButtonUpdateFrames[ctrlButton] == counter))
    {
        m_ButtonUpdateFrames[ctrlButton]--;
        return true;
    }
    return false;
}

float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
    if (IsInputLocked())
        return 0.f;

    if (m_Controller == nullptr)
        return 0.f;

    float roundedAxis = SDL_GameControllerGetAxis(m_Controller, axis) / 32767.f;
    roundedAxis = static_cast<float>(std::ceil(roundedAxis * 10.0) / 10.0);

    return roundedAxis;
}

float sad::InputManager::GetLeftAxis(const std::string& orientation)
{
    SDL_GameControllerAxis axis;

    if (orientation == "Horizontal")
    {
        axis = SDL_CONTROLLER_AXIS_LEFTX;
        return GetAxis(axis);
    }

    if (orientation == "Vertical")
    {
        axis = SDL_CONTROLLER_AXIS_LEFTY;
        return GetAxis(axis);
    }

    return 0.f;
}

float sad::InputManager::GetRightAxis(const std::string& orientation)
{
    SDL_GameControllerAxis axis;

    if (orientation == "Horizontal")
    {
        axis = SDL_CONTROLLER_AXIS_RIGHTX;
        return GetAxis(axis);
    }

    if (orientation == "Vertical")
    {
        axis = SDL_CONTROLLER_AXIS_RIGHTY;
        return GetAxis(axis);
    }

    return 0.f;
}