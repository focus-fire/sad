using System;

namespace Sad
{
    public class Transform
    {
        private readonly Guid GUID;

        protected Transform() { }
        internal Transform(Guid guid) => GUID = guid;

        private Vector3 m_Position;
        public Vector3 position
        {
            get
            {
                Internal.Transform.GetPosition(GUID, out Vector3 position);
                m_Position = position;
                return m_Position;
            }
            set
            {
                Internal.Transform.SetPosition(GUID, ref value);
                m_Position = value;
            }
        }

        private Quaternion m_Rotation;
        public Quaternion rotation
        {
            get
            {
                Internal.Transform.GetRotation(GUID, out Quaternion rotation);
                m_Rotation = rotation;
                return m_Rotation;
            }
            set
            {
                Internal.Transform.SetRotation(GUID, ref value);
                m_Rotation = value;
            }
        }

        private Vector3 m_Scale;
        public Vector3 scale
        {
            get
            { 
                Internal.Transform.GetScale(GUID, out Vector3 scale);
                m_Scale = scale;
                return m_Scale;
            }
            set
            {
                Internal.Transform.SetScale(GUID, ref value);
                m_Scale = value;
            }
        }

        public void Translate(Vector3 translation)
        {
            Internal.Transform.Translate(GUID, ref translation);
        }

        public void Rotate(Vector3 rotation)
        {
            Internal.Transform.Rotate(GUID, ref rotation);
        }

        public void RotateByQuaternion(Quaternion rotation)
        {
            Internal.Transform.RotateByQuaternion(GUID, ref rotation);
        }

        public void Scale(Vector3 scale)
        {
            Internal.Transform.Scale(GUID, ref scale);
        }
    }
}
