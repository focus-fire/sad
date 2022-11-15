using System.Runtime.CompilerServices;

namespace Sad
{
    public static class Core 
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Log(string message);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Warn(string message);
    }
}
