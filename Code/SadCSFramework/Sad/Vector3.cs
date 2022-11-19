using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Sad
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public const float kEpsilon = 0.00001f;
        public const float kEpsilonNormalSqrt = 1e-15f;

        /// <summary>
        /// Creates a Vector with all components as passed arguments 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        /// <summary>
        /// Creates a Vector and sets the x and y components to a passed value and z to 0
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Vector3(float x, float y)
        {
            this.x = x;
            this.y = y;
            z = 0;
        }

        /// <summary>
        /// Creates a Vector and sets all components to the value of a provided scalar
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Vector3(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
        }

        public static Vector3 zero => new Vector3(0.0f);
        public static Vector3 one => new Vector3(1.0f);
        public static Vector3 up => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 down => new Vector3(0.0f, -1.0f, 0.0f);
        public static Vector3 left => new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 right => new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 forward => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 backward => new Vector3(0.0f, 0.0f, -1.0f);

        /// <summary>
        /// Accesses the x, y, and z components of the vector by using the [] operator
        /// </summary>
        public float this[int index]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                switch(index)
                {
                    case 0: return x;
                    case 1: return y;
                    case 2: return z;
                    default:
                        throw new Exception("Invalid Vector3 index!");
                }
            }

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set
            {
                switch(index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    case 2: z = value; break;
                    default:
                        throw new Exception("Invalid Vector3 index!"); 
                }
            }
        }

        /// <summary>
        /// Linearly interpolates two vectors 
        /// </summary>
        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            t = SadMath.NormalizedClamp(t);

            return new Vector3(
                a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t
            );
        }

        /// <summary>
        /// Multiplies the components of two vectors together and returns the result
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Scale(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }

        /// <summary>
        /// Multiplies the components of this vector by another set of components 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Scale(Vector3 b)
        {
            x *= b.x;
            y *= b.y;
            z *= b.z;
        }

        /// <summary>
        /// Calculates the cross product of two vectors and returns it into a new one 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        /// <summary>
        /// Calculates the dot product of two vectors
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(Vector3 a, Vector3 b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z; 
        }

        /// <summary>
        /// Calculates the magnitude of the provided vector 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Magnitude(Vector3 a)
        {
            return (float) SadMath.Sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        }

        /// <summary>
        /// Property for retrieving the magnitude of the current vector
        /// </summary>
        public float magnitude
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get => (float) SadMath.Sqrt(x * x + y * y + z * z);
           
        }

        /// <summary>
        /// Normalizes the components of the provied vector 
        /// </summary>
        public static Vector3 Normalize(Vector3 a)
        {
            float mag = Magnitude(a); 
            if (mag > kEpsilon)
                return a / mag;
            else
                return zero;
        }

        /// <summary>
        /// Normalizes the components of the referenced vector
        /// </summary>
        public Vector3 normalized
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get => Normalize(this);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator+(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator-(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator-(Vector3 a)
        {
            return new Vector3(-a.x, -a.y, -a.z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator*(Vector3 a, float s)
        {
            return new Vector3(a.x * s, a.y * s, a.z * s);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator*(float s, Vector3 a)
        {
            return new Vector3(a.x * s, a.y * s, a.z * s);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator/(Vector3 a, float s)
        {
            return new Vector3(a.x / s, a.y / s, a.z / s);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public override string ToString()
        {
            return "Vector3(" + x + ", " + y + ", " + z + ")" ;
        }
    }
}
