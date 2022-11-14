using System.Runtime.CompilerServices;

namespace Sad
{
    public static class Core
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Log(string message);
    }
}
