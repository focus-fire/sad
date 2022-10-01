#pragma once

#include <SDL2/SDL.h>

namespace sad
{

    class InputManager {
    public:
        void CatchKeyboardEvent(SDL_Event& event);
        void CatchGamepadEvent(SDL_Event& event, SDL_Joystick *joy);

        bool GetKey(int key); // Returns true when key is held
        bool GetKeyPressed(int key); // Returns true if key is pressed
        bool GetKeyReleased(int key); // Returns true when key is released

        bool GetMouseButton(int button); // Returns true when mouse button is held
        bool GetMouseButtonPressed(int button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(int button); // Returns true if mouse button is released

        static sad::InputManager& GetInstance();

        bool KeyForward;
        bool KeyLeft;
        bool KeyRight;
        bool KeyBackward;

    private:
        static sad::InputManager s_Instance;
    };
}