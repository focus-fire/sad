#pragma once

namespace sad
{
    class InputManager {
    public:
        bool GetKey(int key); // Returns true when key is held
        bool GetKeyDown(int key); // Returns true if key is pressed
        bool GetKeyUp(int key); // Returns true when key is released

        bool GetMouseButton(int button); // Returns true when mouse button is held
        bool GetMouseButtonDown(int button); // Returns true if mouse button is pressed
        bool GetMouseButtonUp(int button); // Returns true if mouse button is released

        static sad::InputManager& GetInstance();
    private:
        static sad::InputManager s_Instance;
        InputManager();
        ~InputManager();
    };
}