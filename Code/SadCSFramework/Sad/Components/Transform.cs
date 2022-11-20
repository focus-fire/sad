namespace Sad
{
    public class Transform : Component
    {
        /// <summary>
        /// Retrieves the current position for an entity's transform  
        /// </summary>
        public Vector3 position
        {
            get
            {
                Internal.Transform.GetPosition(Entity.GUID, out Vector3 position);
                return position;
            }
            set
            {
                Internal.Transform.SetPosition(Entity.GUID, ref value);
            }
        }

        /// <summary>
        /// Retrieves the current rotation for an entity's transform as a quaternion
        /// </summary>
        public Quaternion rotation
        {
            get
            {
                Internal.Transform.GetRotation(Entity.GUID, out Quaternion rotation);
                return rotation;
            }
            set
            {
                Internal.Transform.SetRotation(Entity.GUID, ref value);
            }
        }

        /// <summary>
        /// Retrieves the current scale for an entity's transform
        /// </summary>
        public Vector3 scale
        {
            get
            { 
                Internal.Transform.GetScale(Entity.GUID, out Vector3 scale);
                return scale;
            }
            set
            {
                Internal.Transform.SetScale(Entity.GUID, ref value);
            }
        }

        /// <summary>
        /// Translates this transform by a vector 
        /// </summary>
        public void Translate(Vector3 translation)
        {
            Internal.Transform.Translate(Entity.GUID, ref translation);
        }

        /// <summary>
        /// Rotates this transform by a vector 
        /// </summary>
        public void Rotate(Vector3 rotation)
        {
            Internal.Transform.Rotate(Entity.GUID, ref rotation);
        }

        /// <summary>
        /// Rotates this transform by a quaternion 
        /// </summary>
        public void RotateByQuaternion(Quaternion rotation)
        {
            Internal.Transform.RotateByQuaternion(Entity.GUID, ref rotation);
        }

        /// <summary>
        /// Scales this transform by a vector 
        /// </summary>
        public void Scale(Vector3 scale)
        {
            Internal.Transform.Scale(Entity.GUID, ref scale);
        }
    }
}
