using System;
using System.Collections.Generic;

namespace Sad
{
    public class SadBehaviour
    {
        /// <summary>
        /// GUID of the entity this SadBehaviour belongs to
        /// </summary>
        public readonly Guid GUID;

        /// <summary>
        /// Cache for local 'GetComponent' calls.
        /// This is mainly used to reduce heap allocations.
        /// ex: calling 'new T()' everytime we fetch a native component. 
        /// </summary>
        internal Dictionary<Type, object> ComponentCache;

        protected SadBehaviour() => GUID = Guid.Empty;
        internal SadBehaviour(Guid guid)
        { 
            GUID = guid;
            ComponentCache = new Dictionary<Type, object>();
        }

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

        /// <summary>
        /// Retrieves the managed implementation for desired component if it exists  
        /// </summary>
        public T GetComponent<T>() where T: Component, new()
        {
            if (!HasComponent<T>())
                return null;

            Type componentType = typeof(T);
            if (ComponentCache.ContainsKey(componentType))
                return (T) ComponentCache[componentType];

            // Store the component in the cache if it hasn't been instantiated
            T component =  new T() { Entity = this };
            ComponentCache.Add(componentType, component);

            return component;
        }

        /// <summary>
        /// Calls the native implementation for 'HasComponent' on the desired component
        /// </summary>
        public bool HasComponent<T>() where T: Component, new()
        {
            Type componentType = typeof(T);

            return Internal.ECS.HasComponent(GUID, componentType);
        }

        /// <summary>
        /// Calls the native implementation for 'RemoveComponent' on the desired component if it exists on the entity 
        /// </summary>
        public void RemoveComponent<T>() where T: Component, new()
        {
            if (!HasComponent<T>())
                return;

            // Remove the component from the cache 
            Type componentType = typeof(T);
            if (ComponentCache.ContainsKey(componentType))
                ComponentCache.Remove(componentType);

            Internal.ECS.RemoveComponent(GUID, componentType);
        }
    }
}
