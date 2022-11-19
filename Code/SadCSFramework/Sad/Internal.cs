using System;
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

        public static class Transform
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void GetPosition(Guid entityGuid, out Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void SetPosition(Guid entityGuid, ref Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void GetRotation(Guid entityGuid, out Quaternion rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void SetRotation(Guid entityGuid, ref Quaternion rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void GetScale(Guid entityGuid, out Vector3 scale);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void SetScale(Guid entityGuid, ref Vector3 scale);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Translate(Guid entityGuid, ref Vector3 translation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Rotate(Guid entityGuid, ref Vector3 rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void RotateByQuaternion(Guid entityGuid, ref Quaternion rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Scale(Guid entityGuid, ref Vector3 scale);
        }
    }
}
