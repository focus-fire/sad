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
    }
}
