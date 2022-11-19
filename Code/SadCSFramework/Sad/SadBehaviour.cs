using System;

namespace Sad
{
    public class SadBehaviour
    {
        public readonly Guid GUID;

        protected SadBehaviour() => GUID = Guid.Empty;
        internal SadBehaviour(Guid guid)
        { 
            GUID = guid;

            transform = new Transform(GUID);
        }

        /// <summary>
        /// Each SadBehaviour should have a transform by default
        /// </summary>
        public Transform transform;
    }
}
