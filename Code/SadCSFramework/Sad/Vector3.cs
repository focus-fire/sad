using System;
using System.Security.Cryptography.X509Certificates;
using Sad;

namespace Sad
{
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        /// <summary>
        /// Creates a Vector with all components as passed arguments 
        /// </summary>
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        /// <summary>
        /// Creates a Vector and sets the x and y components to a passed value and z to 0
        /// </summary>
        public Vector3(float x, float y)
        {
            this.x = x;
            this.y = y;
            z = 0;
        }

        /// <summary>
        /// Accesses the x, y, and z components of the vector by using the [] operator
        /// </summary>
        public float this[int index]
        {
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
        /// Multiplies the components of two vectors together and returns the result
        /// </summary>
        public static Vector3 Scale(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }

        /// <summary>
        /// Multiplies the components of this vector by another set of components 
        /// </summary>
        public void Scale(Vector3 b)
        {
            x *= b.x;
            y *= b.y;
            z *= b.z;
        }

        /// <summary>
        /// Calculates the cross product of two vectors and returns it into a new one 
        /// </summary>
        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }
    }
}
