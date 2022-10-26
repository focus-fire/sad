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
        bool GetKey(SDL_Scancode key);

        /**
         * @brief Returns true if a keyboard key is pressed during the current frame
         * @param key scancode
         * @return
        */
        bool GetKeyPressed(SDL_Scancode key);

        /**
         * @brief Returns true if a keyboard key is released during the current frame
         * @param key scancode
         * @return
        */
        bool GetKeyReleased(SDL_Scancode key);


        // Mouse Events
        SDL_Point MousePosition;
        bool mouseState[6];
        int  mouseUpdateFrames[6];

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
            MousePosition.x = x;
            MousePosition.y = y;
        }
        SDL_Point GetMousePosition(){ return MousePosition; };


        // Controller Events
        SDL_GameController* controller = nullptr;
        std::unordered_map<int, bool> buttonState;
        std::unordered_map<int, int>  buttonUpdateFrames;
        float ControllerDeadZone;
        bool  ControllerIsActive;

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
        bool GetButton(SDL_GameControllerButton button);

        /**
         * @brief Returns true if a controller button is pressed during the current frame
         * @param button controller button
         * @return
        */
        bool GetButtonPressed(SDL_GameControllerButton button);

        /**
         * @brief Returns true if a controller button is released during the current frame
         * @param button controller button
         * @return
        */
        bool GetButtonReleased(SDL_GameControllerButton button);
        
        /**
         * @brief Returns axis rounded to 1.0 of a given conotroller axis
         * @param axis controller axis
         * @return
        */
        float GetAxis(SDL_GameControllerAxis axis);

    private:
        InputManager() {}
        static sad::InputManager s_InputManager;
    };
}