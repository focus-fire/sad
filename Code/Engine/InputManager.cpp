#include "sadpch.h"

#include "InputManager.h"

#include "ConfigManager.h"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <imgui_internal.h>

sad::InputManager& sad::InputManager::GetInstance()
{
    static sad::InputManager instance;
    return instance;
}

ImGuiID m_currentActiveId = 0;
// Detects if the game window is focused
bool CheckGameFocus()
{
    return m_currentActiveId == 2776656528;
}

// Keyboard Events
void sad::InputManager::CatchKeyboardEvents(SDL_Event& event)
{
    if (ImGui::GetActiveID() != 0)
    {
        m_currentActiveId = ImGui::GetActiveID();
    }
    
    if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat)
    {
        m_ControllerBeingUsed = false;
        m_KeyboardState[event.key.keysym.scancode] = (event.type == SDL_KEYDOWN);
        m_KeyboardUpdateFrames[event.key.keysym.scancode] = SDL_GetTicks64();
    }
}

bool sad::InputManager::GetKey(KeyCode key)
{
    if (!CheckGameFocus())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    return SDL_GetKeyboardState(nullptr)[keyScancode];
}

bool sad::InputManager::GetKeyPressed(KeyCode key)
{
    if (!CheckGameFocus())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    return m_KeyboardState[keyScancode] && (m_KeyboardUpdateFrames[keyScancode] == SDL_GetTicks64());
}

bool sad::InputManager::GetKeyReleased(KeyCode key)
{
    if (!CheckGameFocus())
        return false;

    SDL_Scancode keyScancode = static_cast<SDL_Scancode>(key);
    return !m_KeyboardState[keyScancode] && (m_KeyboardUpdateFrames[keyScancode] == SDL_GetTicks64());
}

// Mouse Events

void sad::InputManager::CatchMouseEvents(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_ControllerBeingUsed = false;
        m_MouseState[event.button.button] = (event.type == SDL_MOUSEBUTTONDOWN);
        m_MouseUpdateFrames[event.button.button] = SDL_GetTicks64();
    }
}

bool sad::InputManager::GetMouseButton(int button)
{
    return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button));
}

bool sad::InputManager::GetMouseButtonPressed(int button)
{
    return m_MouseState[button] && (m_MouseUpdateFrames[button] == SDL_GetTicks64());
}

bool sad::InputManager::GetMouseButtonReleased(int button)
{
    return !m_MouseState[button] && (m_MouseUpdateFrames[button] == SDL_GetTicks64());
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
    if (GetLeftAxis("Horizontal") > ControllerDeadZone || GetLeftAxis("Vertical") > ControllerDeadZone
        || GetRightAxis("Horizontal") > ControllerDeadZone || GetRightAxis("Vertical") > ControllerDeadZone)
    {
        m_ControllerBeingUsed = true;
    }

    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
    {
        m_ControllerBeingUsed = true;
        m_ButtonState[event.cbutton.button] = (event.type == SDL_CONTROLLERBUTTONDOWN);
        m_ButtonUpdateFrames[event.cbutton.button] = SDL_GetTicks64();
    }
}

bool sad::InputManager::GetButton(ControllerButton button)
{
    if (m_Controller == nullptr)
        return false;

    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    return SDL_GameControllerGetButton(m_Controller, ctrlButton);
}

bool sad::InputManager::GetButtonPressed(ControllerButton button)
{
    if (m_Controller == nullptr)
        return false;

    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    return m_ButtonState[ctrlButton] && (m_ButtonUpdateFrames[ctrlButton] == SDL_GetTicks64());
}

bool sad::InputManager::GetButtonReleased(ControllerButton button)
{
    if (m_Controller == nullptr)
        return false;
    SDL_GameControllerButton ctrlButton = static_cast<SDL_GameControllerButton>(button);

    return !m_ButtonState[ctrlButton] && (m_ButtonUpdateFrames[ctrlButton] == SDL_GetTicks64());
}

float sad::InputManager::GetAxis(SDL_GameControllerAxis axis)
{
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