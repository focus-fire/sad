using System;

namespace Sad
{
    public class Transform
    {
        private readonly Guid GUID;

        public Transform() { }
        internal Transform(Guid guid) => GUID = guid;

        private Vector3 m_Position;
        public Vector3 Position
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
    }
}
