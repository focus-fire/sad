using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Sad
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        public float w;
        public float x;
        public float y;
        public float z;

        public const float kEpsilon = 0.000001f;

        /// <summary>
        /// Creates a quaternion with all components passed as arguments 
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Quaternion(float w, float x, float y, float z)
        {
            this.w = w;
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Quaternion identity => new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

        /// <summary>
        /// Accesses the w, x, y, and z components of the quaternion by using the [] operator
        /// </summary>
        public float this[int index]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                switch(index)
                {
                    case 0: return w;
                    case 1: return x;
                    case 2: return y;
                    case 3: return z;
                    default:
                        throw new Exception("Invalid Vector3 index!");
                }
            }

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set
            {
                switch(index)
                {
                    case 0: w = value; break;
                    case 1: x = value; break;
                    case 2: y = value; break;
                    case 3: z = value; break;
                    default:
                        throw new Exception("Invalid Vector3 index!"); 
                }
            }
        }

        /// <summary>
        /// Calculates the dot product of two quaternions
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
        }

        public static bool IsEqualWithDot(float dot)
        {
            return dot > 1.0f - kEpsilon;
        }

        /// <summary>
        /// Calculates the angle in degrees between a and b
        /// </summary>
        public static float Angle(Quaternion a, Quaternion b)
        {
            float dot = SadMath.Min(SadMath.Abs(Dot(a, b)), 1.0f);
            return IsEqualWithDot(dot) ? 0.0f : (float) Math.Acos(dot) * 2.0f * SadMath.RadToDeg;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Quaternion operator*(Quaternion a, Quaternion b)
        {
            return new Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
                a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
            );
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator*(Quaternion rotation, Vector3 point)
        {
            float x = rotation.x * 2.0f;
            float y = rotation.y * 2.0f;
            float z = rotation.z * 2.0f;
            float xx = rotation.x * x;
            float yy = rotation.y * y;
            float zz = rotation.z * z;
            float xy = rotation.x * y;
            float xz = rotation.x * z;
            float yz = rotation.y * z;
            float wx = rotation.w * x;
            float wy = rotation.w * y;
            float wz = rotation.w * z;

            Vector3 res;

            res.x = (1F - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;
            res.y = (xy + wz) * point.x + (1F - (xx + zz)) * point.y + (yz - wx) * point.z;
            res.z = (xz - wy) * point.x + (yz + wx) * point.y + (1F - (xx + yy)) * point.z;

            return res;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public override string ToString()
        {
            return "Quaternion(" + w + ", " + x + ", " + y + ", " + z + ")" ;
        }
    }
}
