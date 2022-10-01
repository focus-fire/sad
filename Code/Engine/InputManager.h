#pragma once

namespace sad
{

    class InputManager {
    public:
        void Update();

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
        InputManager();
        ~InputManager();
    };
}