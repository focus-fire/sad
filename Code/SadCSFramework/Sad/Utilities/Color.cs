using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Sad
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public float r;
        public float g;
        public float b;
        public float a;

        /// <summary>
        /// Initializes a Color with r, g, b, and a values 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Color(float r, float g, float b, float a = 1.0f)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }

        public static Color red => new Color(1.0f, 0.0f, 0.0f, 1.0f);
        public static Color green => new Color(0.0f, 1.0f, 0.0f, 1.0f);
        public static Color blue => new Color(0.0f, 0.0f, 1.0f, 1.0f);
        public static Color white => new Color(1.0f, 1.0f, 1.0f, 1.0f);
        public static Color black => new Color(0.0f, 0.0f, 0.0f, 1.0f);
        public static Color cyan => new Color(0.0f, 1.0f, 1.0f, 1.0f);
        public static Color magenta => new Color(1.0f, 0.0f, 1.0f, 1.0f);

        /// <summary>
        /// Accesses the r, g, b, and a components of the color by using the [] operator
        /// </summary>
        public float this[int index]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                switch(index)
                {
                    case 0: return r;
                    case 1: return g;
                    case 2: return b;
                    case 3: return a;
                    default:
                        throw new Exception("Invalid Vector3 index!");
                }
            }

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set
            {
                switch(index)
                {
                    case 0: r = value; break;
                    case 1: g = value; break;
                    case 2: b = value; break;
                    case 3: a = value; break;
                    default:
                        throw new Exception("Invalid Vector3 index!"); 
                }
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public override string ToString()
        {
            return "RGBA(" + r + ", " + g + ", " + b + ", " + a + ")" ;
        }
    }
}
