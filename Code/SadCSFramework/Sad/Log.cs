namespace Sad
{
    public class Log
    {
        public static void Debug(string message) => Internal.Log.Debug(message);
        public static void Warn(string message) => Internal.Log.Warn(message);
        public static void Error(string message) => Internal.Log.Error(message);
    }
}
