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
        }
    }
}
