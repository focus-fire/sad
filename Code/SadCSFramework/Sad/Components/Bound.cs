using System;

namespace Sad
{
    public class Bound : Component
    {
        /// <summary>
        /// Retrieves the minimum extent for this entity's bound
        /// </summary>
        public Vector3 min
        {
            get 
            { 
                Internal.Bound.GetBoundMin(EntityGuid, out Vector3 boundMin);
                return boundMin;
            }
        }

        /// <summary>
        /// Retrieves the maximum extent for this entity's bound
        /// </summary>
        public Vector3 max
        {
            get
            {
                Internal.Bound.GetBoundMax(EntityGuid, out Vector3 boundMax);
                return boundMax;
            }
        }

        public bool Intersects(Entity target)
        {
            return Internal.Bound.Intersects(EntityGuid, target.GUID); 
        }
    }
}
