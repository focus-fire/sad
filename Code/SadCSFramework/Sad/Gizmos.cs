namespace Sad
{
    public class Gizmos
    {
        public static void DrawLine(Vector3 from, Vector3 to, Vector3 color) 
        {
            Internal.Gizmos.DrawLine(ref from, ref to, ref color);
        }

        public static void DrawBox(Vector3 min, Vector3 max, Vector3 color)
        {
            Internal.Gizmos.DrawBox(ref min, ref max, ref color);
        }
    }
}
