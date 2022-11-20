using System;
using System.Collections.Generic;

namespace Sad
{
    public class SadBehaviour : Entity
    {
        protected SadBehaviour() : base() { }

        internal SadBehaviour(Guid guid) : base(guid) { }

        /// <summary>
        /// Each SadBehaviour should have a transform by default
        /// </summary>
        private Transform m_Transform;
        public Transform transform
        {
            get
            {
                if (m_Transform == null)
                    m_Transform = GetComponent<Transform>();

                return m_Transform;
            }
        }

        /// <summary>
        /// Each SadBehaviour should have a bound by default
        /// </summary>
        private Bound m_Bound;
        public Bound bound
        {
            get
            {
                if (m_Bound == null)
                    m_Bound = GetComponent<Bound>();

                return m_Bound;
            }
        }

    }
}
