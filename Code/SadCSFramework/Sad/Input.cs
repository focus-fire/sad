namespace Sad
{
    public class Input
    {
        public static bool GetButton(ControllerButton button) => Internal.Input.GetButton(button);

        public static bool GetButtonPressed(ControllerButton button) => Internal.Input.GetButtonPressed(button);

        public static bool GetButtonReleased(ControllerButton button) => Internal.Input.GetButtonReleased(button);

        public static bool GetKey(KeyCode key) => Internal.Input.GetKey(key);

        public static bool GetKeyPressed(KeyCode key) => Internal.Input.GetKeyPressed(key);

        public static bool GetKeyReleased(KeyCode key) => Internal.Input.GetKeyReleased(key);

        public static bool GetMouseButton(int button) => Internal.Input.GetMouseButton(button);

        public static bool GetMouseButtonPressed(int button) => Internal.Input.GetMouseButtonPressed(button);

        public static bool GetMouseButtonReleased(int button) => Internal.Input.GetMouseButtonReleased(button);

        public static MousePosition GetMousePosition() => Internal.Input.GetMousePosition();

        public static bool IsControllerActive() => Internal.Input.IsControllerActive();

        public static bool IsControllerConnected() => Internal.Input.IsControllerConnected();

        public static float GetLeftAxis(string orientation) => Internal.Input.GetLeftAxis(orientation);

        public static float GetRightAxis(string orientation) => Internal.Input.GetRightAxis(orientation);
    }
}
