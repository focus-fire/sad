using System.Runtime.CompilerServices;


namespace Sad
{
    namespace Internal
    {
        public static class Log 
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Debug(string message);
            
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Warn(string message);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Error(string message);
        }

        public static class Audio
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void PlaySFX(string audioString);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void PlayMusic(string audioString, int times);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void SetVolume(string audioString, int volume);
        }

        public static class Input
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetButton(ControllerButton button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetButtonPressed(ControllerButton button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetButtonReleased(ControllerButton button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetKey(KeyCode key);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetKeyPressed(KeyCode key);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetKeyReleased(KeyCode key);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetMouseButton(int button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetMouseButtonPressed(int button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool GetMouseButtonReleased(int button);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static MousePosition GetMousePosition();

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool IsControllerActive();

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool IsControllerConnected();

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static float GetLeftAxis(string orientation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static float GetRightAxis(string orientation);
        }

        public static class Misc
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static float Getdt();
        }
    }
}
