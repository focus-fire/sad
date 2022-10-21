#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace sad
{

    class InputManager {

        enum class Button
        {
            ButtonUp,
            ButtonDown,
            ButtonLeft,
            ButtonRight,
            DPadUp,
            DPadDown,
            DpadLeft,
            DpadRight,
            L1,
            L2,
            R1,
            R2
        };

        enum class Axis
        {
            LeftStickUp,
            LeftStickDown,
            LeftStickLeft,
            LeftStickRight,
            RightStickUp,
            RightStickDown,
            RightStickLeft,
            RightStickRight
        };

    public:
        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;

        // Keyboard Events
        void CatchKeyboardEvent(SDL_Event& event);
        
        bool GetKey(SDL_Scancode key); // Returns true if key is held
        bool GetKeyPressed(SDL_Scancode key); // Returns true if key is pressed
        bool GetKeyReleased(SDL_Scancode key); // Returns true if key is released

        bool GetMouseButton(SDL_MouseButtonEvent button); // Returns true if mouse button is held
        bool GetMouseButtonPressed(SDL_MouseButtonEvent button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(SDL_MouseButtonEvent button); // Returns true if mouse button is released

        const Uint8* CurrentKeyboardStates = SDL_GetKeyboardState(nullptr);
        std::map<SDL_Scancode, bool> KeyboardStates;

        // Controller Events
        void CatchGamepadEvent(SDL_Event& event, SDL_Joystick* joy);

        bool ControllerIsActive = false;
        void OnControllerConnected(SDL_ControllerDeviceEvent& e);
        void OnControllerDisconnected(SDL_ControllerDeviceEvent& e);

        bool GetButton(Button button); // Returns true if button is held
        bool GetButtonPressed(Button button); // Returns true if button is pressed
        bool GetButtonReleased(Button button); // Returns true if button is released
        float GetAxis(Axis axis);

        SDL_GameController* controller = nullptr;

        /**
         * @brief Saves the down state of a key.
         * @param key 
         * @param pressed 
        */
        void UpdateKeyboardState(SDL_Scancode key, bool pressed)
        {
            std::map<SDL_Scancode, bool>::iterator iter = KeyboardStates.find(key);
            if (iter != KeyboardStates.end())
            {
                iter->second = pressed;
                return;
            }
            KeyboardStates.insert({key, pressed});
        }

        /**
         * @brief Returns true if a key is down in the previous frame.
         * @param key 
         * @return 
        */
        bool GetKeyboardState(SDL_Scancode key)
        {
            std::map<SDL_Scancode, bool>::iterator iter = KeyboardStates.find(key);
            if (iter != KeyboardStates.end())
            {
                return iter->second;
            }
            return false;
        }

    private:
        InputManager() {}
        static sad::InputManager s_InputManager;
    };
}