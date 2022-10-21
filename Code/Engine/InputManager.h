#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace sad
{

    class InputManager {
    public:
        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;

        // Keyboard Events
        bool GetKey(SDL_Scancode key); // Returns true if key is held
        bool GetKeyPressed(SDL_Scancode key); // Returns true if key is pressed
        bool GetKeyReleased(SDL_Scancode key); // Returns true if key is released

        bool GetMouseButton(SDL_MouseButtonEvent button); // Returns true if mouse button is held
        bool GetMouseButtonPressed(SDL_MouseButtonEvent button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(SDL_MouseButtonEvent button); // Returns true if mouse button is released

        const Uint8* CurrentKeyboardStates = SDL_GetKeyboardState(nullptr);
        std::map<SDL_Scancode, bool> KeyboardStates;

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
            KeyboardStates.insert({ key, pressed });
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

        // Controller Events
        bool ControllerIsActive = false;
        void OnControllerConnected(SDL_ControllerDeviceEvent& e);
        void OnControllerDisconnected(SDL_ControllerDeviceEvent& e);

        bool GetButton(SDL_GameControllerButton button); // Returns true if button is held
        bool GetButtonPressed(SDL_GameControllerButton button); // Returns true if button is pressed
        bool GetButtonReleased(SDL_GameControllerButton button); // Returns true if button is released
        float GetAxis(SDL_GameControllerAxis axis);

        SDL_GameController* controller = nullptr;
        std::map<SDL_GameControllerButton, bool> ButtonStates;

        /**
         * @brief Saves the down state of a controller button.
         * @param button
         * @param pressed
        */
        void UpdateControllerButtonState(SDL_GameControllerButton button, bool pressed)
        {
            std::map<SDL_GameControllerButton, bool>::iterator iter = ButtonStates.find(button);
            if (iter != ButtonStates.end())
            {
                iter->second = pressed;
                return;
            }
            ButtonStates.insert({ button, pressed });
        }

        /**
         * @brief Returns true if a controller button is down in the previous frame.
         * @param button
         * @return
        */
        bool GetControllerButtonState(SDL_GameControllerButton button)
        {
            std::map<SDL_GameControllerButton, bool>::iterator iter = ButtonStates.find(button);
            if (iter != ButtonStates.end())
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