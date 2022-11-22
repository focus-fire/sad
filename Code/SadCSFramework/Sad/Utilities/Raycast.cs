namespace Sad
{
    public class Raycast
    {
        public static bool Intersects(Vector3 origin, Vector3 direction, Entity entity)
        {
            Internal.Transform.GetPosition(entity.GUID, out Vector3 position);
            Internal.Transform.GetScale(entity.GUID, out Vector3 scale);
            return Internal.Misc.RaycastIntersection(ref origin, ref direction, ref position, ref scale);
        }
    }
}
