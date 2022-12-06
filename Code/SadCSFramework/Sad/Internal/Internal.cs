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
            /* Entity Queries */

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static Guid FindEntityByName(string entityName);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static Guid Instantiate(string entityName);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static Guid InstantiateWithModel(string entityName, string modelResourceName);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static Guid InstantiateWithModelAndPosition(string entityName, string modelResourceName, ref Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void DestroyEntityByName(string entityName);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void DestroyEntityByGuid(Guid entityName);

            /* Component Queries */

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool HasComponent(Guid entityGuid, Type componentType);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void AddComponent(Guid entityGuid, Type componentType);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void RemoveComponent(Guid entityGuid, Type componentType);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static object GetScriptInstance(Guid entityGuid);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool HasScriptInstance(Guid entityGuid);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void AddScriptInstance(Guid entityGuid, string scriptName);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void RemoveScriptInstance(Guid entityGuid);
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
            
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static Quaternion Slerp(Quaternion quatStart, Quaternion quatEnd, float interpFactor);

			[MethodImplAttribute(MethodImplOptions.InternalCall)]
			internal extern static Quaternion LookAt(Vector3 eye, Vector3 center, Vector3 up);
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

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool Intersects(Guid aGuid, Guid bGuid);
        }

        /// <summary>
        /// Internal methods for the native Audio helpers
        /// </summary>
        public static class Audio
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void PlaySFX(string audioString);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void PlayMusic(string audioString, int times);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void SetVolume(string audioString, int volume);
        }

        /// <summary>
        /// Internal methods for the native Input helpers
        /// </summary>
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

        /// <summary>
        /// Internal helper methods for native utilities 
        /// </summary>
        public static class Misc
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static float Getdt();

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static bool RaycastIntersection(ref Vector3 origin, ref Vector3 direction, ref Vector3 position, ref Vector3 size);
        }
    }
}
