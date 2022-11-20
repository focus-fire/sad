using System;
using System.Runtime.CompilerServices;

namespace Sad
{
    namespace Internal
    {
        /// <summary>
        /// Internal methods for all native ECS operations 
        /// </summary>
        internal static class ECS
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool HasComponent(Guid entityGuid, Type componentType);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void AddComponent(Guid entityGuid, Type componentType);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void RemoveComponent(Guid entityGuid, Type componentType);
        }

        /// <summary>
        /// Internal methods for all native Log operations
        /// </summary>
        internal static class Log 
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Debug(string message);
            
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Warn(string message);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Error(string message);
        }

        /// <summary>
        /// Internal methods for all native Gizmo operations
        /// </summary>
        internal static class Gizmos
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void DrawLine(ref Vector3 from, ref Vector3 to, ref Color color);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void DrawBox(ref Vector3 min, ref Vector3 max, ref Color color);
        }

        /// <summary>
        /// Internal methods for all native Transforms
        /// </summary>
        internal static class Transform
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

        /// <summary>
        /// Internal methods for all native Bounds
        /// </summary>
        internal static class Bound
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void GetBoundMax(Guid entityGuid, out Vector3 boundMax);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void GetBoundMin(Guid entityGuid, out Vector3 boundMin);
        }
    }
}
