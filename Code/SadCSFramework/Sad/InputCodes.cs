namespace Sad
{
    public struct MousePosition
    {
        public int x;
        public int y;
    }

    public class MouseButton
    {
        public const int Left = 1;
        public const int Middle = 2;
        public const int Right = 3;
    }

    public enum ControllerButton
    {
        South,
        North,
        East,
        West,

        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight,

        Start,

        L1,
        R1,

        LeftStick,
        RightStick
    };

    public enum KeyCode
    {
        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,

        Space = 44,
        Escape = 41,

        LShift = 225,
        RShift = 229,

        LCtrl = 224,
        RCtrl = 228,

        RightArrow = 79,
        LeftArrow = 80,
        DownArrow = 81,
        UpArrow = 82
    };
}