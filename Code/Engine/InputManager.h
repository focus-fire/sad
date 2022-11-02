#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace sad
{

    enum class ControllerButton
    {
        South = SDL_CONTROLLER_BUTTON_A,
        North = SDL_CONTROLLER_BUTTON_Y,
        East = SDL_CONTROLLER_BUTTON_B,
        West = SDL_CONTROLLER_BUTTON_X,

        DPadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
        DPadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        DPadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        DPadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

        Start = SDL_CONTROLLER_BUTTON_START,

        L1 = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        R1 = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

        LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK
    };

    enum class KeyCode
    {
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,

        Space = SDL_SCANCODE_SPACE,
        Escape = SDL_SCANCODE_ESCAPE,

        LShift = SDL_SCANCODE_LSHIFT,
        RShift = SDL_SCANCODE_RSHIFT,

        LCtrl = SDL_SCANCODE_LCTRL,
        RCtrl = SDL_SCANCODE_RCTRL,

        RightArrow = SDL_SCANCODE_RIGHT,
        LeftArrow = SDL_SCANCODE_LEFT,
        DownArrow = SDL_SCANCODE_DOWN,
        UpArrow = SDL_SCANCODE_UP
    };

    class InputManager {

    public:
        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;

        // Keyboard Events

        /**
         * @brief Catches keyboard events and updates their states
         * @param event 
        */
        void CatchKeyboardEvents(SDL_Event& event);

        /**
         * @brief Returns true if a keyboard key is held.
         * @param key scancode
         * @return 
        */
        bool GetKey(KeyCode key);

        /**
         * @brief Returns true if a keyboard key is pressed during the current frame
         * @param key scancode
         * @return
        */
        bool GetKeyPressed(KeyCode key);

        /**
         * @brief Returns true if a keyboard key is released during the current frame
         * @param key scancode
         * @return
        */
        bool GetKeyReleased(KeyCode key);

        // Mouse Events

        /**
         * @brief Catches mouse button events and updates their states
         * @param event
        */
        void CatchMouseEvents(SDL_Event& event);
        
        /**
         * @brief Returns true if a mouse button is held.
         * @param button mouse button id
         * @return
        */
        bool GetMouseButton(int button);

        /**
         * @brief Returns true if a mouse button is pressed during the current frame.
         * @param button mouse button id
         * @return
        */
        bool GetMouseButtonPressed(int button);

        /**
         * @brief Returns true if a mouse button is released during the current frame.
         * @param button mouse button id
         * @return
        */
        bool GetMouseButtonReleased(int button); 

        void SetMousePosition(int x, int y) {
            m_MousePosition.x = x;
            m_MousePosition.y = y;
        }

        SDL_Point GetMousePosition(){ return m_MousePosition; };

        // Controller Events
        float ControllerDeadZone;

        /**
         * @brief Handles new controller device
         * @param event
        */
        void OnControllerConnected(SDL_ControllerDeviceEvent& event);
        
        /**
         * @brief Handles controller disconnection
        */
        void OnControllerDisconnected();

        /**
         * @brief Catches controller button events and updates their states
         * param event
        */
        void CatchControllerEvents(SDL_Event& event);

        /**
         * @brief Returns true if a controller button is held
         * @param button controller button
         * @return
        */
        bool GetButton(ControllerButton button);

        /**
         * @brief Returns true if a controller button is pressed during the current frame
         * @param button controller button
         * @return
        */
        bool GetButtonPressed(ControllerButton button);

        /**
         * @brief Returns true if a controller button is released during the current frame
         * @param button controller button
         * @return
        */
        bool GetButtonReleased(ControllerButton button);
        
        /**
         * @brief Returns axis rounded to 1.0 of a given controller axis
         * @param axis controller axis
         * @return
        */
        float GetAxis(SDL_GameControllerAxis axis);

        /**
         * @brief Gets axis of left joystick
         * @param orientation "Horizontal" or "Vertical"
         * @return 
        */
        float GetLeftAxis(const std::string& orientation);

        /**
         * @brief Gets axis of right joystick
         * @param orientation "Horizontal" or "Vertical"
         * @return
        */
        float GetRightAxis(const std::string& orientation);

    private:
        InputManager() {}
        static sad::InputManager s_InputManager;

        std::unordered_map<SDL_Scancode, bool> m_KeyboardState;
        std::unordered_map<SDL_Scancode, Uint64>  m_KeyboardUpdateFrames;

        std::unordered_map<int, bool> m_ButtonState;
        std::unordered_map<int, int>  m_ButtonUpdateFrames;

        SDL_Point m_MousePosition;
        bool m_MouseState[6];
        int  m_MouseUpdateFrames[6];

        SDL_GameController* m_Controller = nullptr;
        bool  m_ControllerIsActive;
    };
}