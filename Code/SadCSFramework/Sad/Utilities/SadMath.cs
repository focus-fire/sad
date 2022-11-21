using System;

namespace Sad
{
    public struct SadMath
    {
        /// <summary>
        /// Returns the absolute value of a float
        /// </summary>
        public static float Abs(float a) => Math.Abs(a);

        /// <summary>
        /// Returns the absolute value of an integer 
        /// </summary>
        public static int Abs(int a) => Math.Abs(a);

        /// <summary>
        /// Returns the square root of a float 
        /// </summary>
        public static float Sqrt(float a) => (float) Math.Sqrt(a);

        /// <summary>
        /// Returns the minimum float of two values 
        /// </summary>
        public static float Min(float a, float b) => a < b ? a : b;

        /// <summary>
        /// Returns the maximum float of two values 
        /// </summary>
        public static float Max(float a, float b) => a > b ? a : b; 

        /// <summary>
        /// Returns the minimum int of two values 
        /// </summary>
        public static int Min(int a, int b) => a < b ? a : b;

        /// <summary>
        /// Returns the maximum int of two values 
        /// </summary>
        public static int Max(int a, int b) => a > b ? a : b;

        /// <summary>
        /// The number people learn to recite for fun 
        /// </summary>
        public const float PI = (float) Math.PI;

        /// <summary>
        /// The number people learn to recite for fun multiplied by two
        /// </summary>
        public const float PI2 = (float) Math.PI * 2;

        /// <summary>
        /// Converts degrees to radians
        /// </summary>
        public const float DegToRad = PI * 2.0f / 360.0f;

        /// <summary>
        /// Converts radians to degrees
        /// </summary>
        public const float RadToDeg = 1.0f / DegToRad;

        /// <summary>
        /// CS representation of positive infinity
        /// </summary>
        public const float Infinity = Single.PositiveInfinity;

        /// <summary>
        /// CS Representation of negative infinity
        /// </summary>
        public const float NegativeInfinity = Single.NegativeInfinity;

        /// <summary>
        /// Clamps a float x between a min and max value  
        /// </summary>
        public static float Clamp(float x, float min, float max)
        {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        /// <summary>
        /// Clamps an int x between a min and max value  
        /// </summary>
        public static int Clamp(int x, int min, int max)
        {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        /// <summary>
        /// Clamps a value between 0 and 1 
        /// </summary>
        public static float NormalizedClamp(float x)
        {
            if (x < 0.0f) return 0.0f;
            if (x > 1.0f) return 1.0f;
            return x;
        }

        /// <summary>
        /// Interpolates a and b by t 
        /// </summary>
        public static float Lerp(float a, float b, float t)
        {
            return a + (b - a) * NormalizedClamp(t);
        }

        /// <summary>
        /// Moves value x towards y
        /// </summary>
        public static float MoveTowards(float x, float y, float delta)
        {
            if (Abs(y - x) <= delta)
                return y;

            return x + Math.Sign(y - x) * delta;
        }
    }
}
