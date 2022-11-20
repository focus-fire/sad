namespace Sad
{
    public class Gizmos
    {
        public static void DrawLine(Vector3 from, Vector3 to, Color color) 
        {
            Internal.Gizmos.DrawLine(ref from, ref to, ref color);
        }

        public static void DrawBox(Vector3 min, Vector3 max, Color color)
        {
            Internal.Gizmos.DrawBox(ref min, ref max, ref color);
        }
    }
}
