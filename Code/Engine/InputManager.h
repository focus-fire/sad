#pragma once

#include <SDL2/SDL.h>

namespace sad
{

    class InputManager {
    public:
        enum Direction {up, down, left, right};

        void CatchKeyboardEvent(SDL_Event& event);
        void CatchGamepadEvent(SDL_Event& event, SDL_Joystick *joy);

        bool GetKey(Direction key); // Returns true when key is held
        bool GetKeyPressed(Direction key); // Returns true if key is pressed
        bool GetKeyReleased(Direction key); // Returns true when key is released

        bool GetMouseButton(int button); // Returns true when mouse button is held
        bool GetMouseButtonPressed(int button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(int button); // Returns true if mouse button is released

        static bool KeyUp;
        static bool KeyDown;
        static bool KeyLeft;
        static bool KeyRight;

        bool* GetKeyState(Direction Key) {
            switch (Key)
            {
                case up:
                    return &KeyUp;

                case down:
                    return &KeyDown;

                case left:
                    return &KeyLeft;

                case right:
                    return &KeyRight;
            }
        }

        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;

    private:
        InputManager() {}
        static sad::InputManager s_InputManager;
    };
}