#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace sad
{

    class InputManager {
    public:
        void CatchKeyboardEvent(SDL_Event& event);
        void CatchGamepadEvent(SDL_Event& event, SDL_Joystick *joy);

        bool GetKey(SDL_Scancode key); // Returns true when key is held
        bool GetKeyPressed(SDL_Scancode key); // Returns true if key is pressed
        bool GetKeyReleased(SDL_Scancode key); // Returns true when key is released

        bool GetMouseButton(SDL_MouseButtonEvent button); // Returns true when mouse button is held
        bool GetMouseButtonPressed(SDL_MouseButtonEvent button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(SDL_MouseButtonEvent button); // Returns true if mouse button is released

        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;
        
        const Uint8* CurrentKeyboardStates = SDL_GetKeyboardState(nullptr);
        std::map<SDL_Scancode, bool> KeyboardStates;
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