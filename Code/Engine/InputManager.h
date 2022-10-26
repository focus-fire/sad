#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace sad
{
    class InputManager {

    public:
        static sad::InputManager& GetInstance();
        InputManager(const InputManager&) = delete;

        int UpdateCounter = 0;
        void IncrementUpdateCounter()
        {
            UpdateCounter++;
        }

        // Keyboard Events
        std::unordered_map<SDL_Scancode, bool> keyboardState;
        std::unordered_map<SDL_Scancode, int>  keyboardUpdateFrames;

        void CatchKeyboardEvents(SDL_Event& event);

        bool GetKey(SDL_Scancode key); // Returns true if key is held
        bool GetKeyPressed(SDL_Scancode key); // Returns true if key is pressed
        bool GetKeyReleased(SDL_Scancode key); // Returns true if key is released


        // Mouse Events
        SDL_Point MousePosition;
        bool mouseState[6];
        int  mouseUpdateFrames[6];

        void CatchMouseEvents(SDL_Event& event);
        
        bool GetMouseButton(int button); // Returns true if mouse button is held
        bool GetMouseButtonPressed(int button); // Returns true if mouse button is pressed
        bool GetMouseButtonReleased(int button); // Returns true if mouse button is released

        void SetMousePosition(int x, int y);
        SDL_Point GetMousePosition();


        // Controller Events
        SDL_GameController* controller = nullptr;
        std::unordered_map<int, bool> buttonState;
        std::unordered_map<int, int>  buttonUpdateFrames;
        float ControllerDeadZone;
        bool  ControllerIsActive;

        void OnControllerConnected(SDL_ControllerDeviceEvent& e);
        void OnControllerDisconnected(SDL_ControllerDeviceEvent& e);

        void CatchControllerEvents(SDL_Event& event);

        bool GetButton(SDL_GameControllerButton button); // Returns true if button is held
        bool GetButtonPressed(SDL_GameControllerButton button); // Returns true if button is pressed
        bool GetButtonReleased(SDL_GameControllerButton button); // Returns true if button is released
        
        float GetAxis(SDL_GameControllerAxis axis); // Returns the value rounded to 1.0f of a given axis

    private:
        InputManager() {}
        static sad::InputManager s_InputManager;
    };
}