using System;
using System.Collections.Generic;

namespace Sad
{
    public class Entity
    {
        /// <summary>
        /// GUID of the entity this SadBehaviour belongs to
        /// </summary>
        public Guid GUID { get; internal set; }

        /// <summary>
        /// Creates an empty entity 
        /// </summary>
        protected Entity() => GUID = Guid.Empty;

        /// <summary>
        /// Creates an entity with a native GUID 
        /// </summary>
        internal Entity(Guid guid) 
        { 
            GUID = guid;
            ComponentCache = new Dictionary<Type, object>();
        }

        /// <summary>
        /// Cache for local 'GetComponent' calls.
        /// This is mainly used to reduce heap allocations.
        /// ex: calling 'new T()' everytime we fetch a native component. 
        /// </summary>
        internal Dictionary<Type, object> ComponentCache;

        /// <summary>
        /// Calls the native implementation for 'HasComponent' on the desired component
        /// </summary>
        public bool HasComponent<T>() where T: Component, new()
        {
            Type componentType = typeof(T);

            return Internal.ECS.HasComponent(GUID, componentType);
        }

        /// <summary>
        /// Checks if the entity has a script instance
        /// </summary>
        public bool HasScriptComponent<T>() where T: SadBehaviour, new()
        {
            return Internal.ECS.HasScriptInstance(GUID);
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
            T component =  new T() { EntityGuid = GUID };
            ComponentCache.Add(componentType, component);

            return component;
        }

        /// <summary>
        /// Attempts to retrieve a managed script off of an object 
        /// </summary>
        public T GetScriptComponent<T>() where T: SadBehaviour, new()
        {
            if (!HasScriptComponent<T>())
                return null;

            Type componentType = typeof(T);
            if (ComponentCache.ContainsKey(componentType))
                return (T) ComponentCache[componentType];

            // Store the component in the cache if it hasn't been instantiated 
            T instance = Internal.ECS.GetScriptInstance(GUID) as T;
            ComponentCache.Add(componentType, instance);

            return instance;
        }

        /// <summary>
        /// Adds the managed implementation of a desired component if it exists
        /// Note: This does not add scripted components (in C#) yet...
        /// </summary>
        public void AddComponent<T>() where T: Component, new()
        {
            if (HasComponent<T>())
                return;

            Type componentType = typeof(T);

            Internal.ECS.AddComponent(GUID, componentType);
        }

        /// <summary>
        /// Adds a script component to the current entity in the mono runtime
        /// </summary>
        public void AddScriptComponent<T>() where T: SadBehaviour, new()
        {
            if (HasScriptComponent<T>())
                return;

            string scriptName = typeof(T).FullName;

            Internal.ECS.AddScriptInstance(GUID, scriptName);
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

        /// <summary>
        /// Removes a script component from the current entity in the Mono runtime
        /// </summary>
        public void RemoveScriptComponent<T>() where T: SadBehaviour, new()
        {
            if (!HasScriptComponent<T>())
                return;

            // Remove the component from the cache
            Type scriptType = typeof(T);
            if (ComponentCache.ContainsKey(scriptType))
                ComponentCache.Remove(scriptType);

            Internal.ECS.RemoveScriptInstance(GUID);
        }

        /// <summary>
        /// Searches the level for an entity with the specified name 
        /// </summary>
        /// <param name="name">Name of the entity being searched for</param>
        /// <returns>Reference to the entity in the level, null if the entity was not found</returns>
        public static Entity FindEntityWithName(string name)
        {
            Guid entityGuid = Internal.ECS.FindEntityByName(name);

            if (entityGuid == Guid.Empty)
                return null;

            return new Entity(entityGuid);
        }

        /// <summary>
        /// Instantiates an entity with the provided name into the level
        /// All newly instantiated entities have a GUID, Name, Transform, and Bound by default
        /// </summary>
        /// <param name="entityName">Name of the entity being instantiated</param>
        /// <returns>Reference to the entity instantiated in the level</returns>
        public Entity Instantiate(string entityName)
        { 
            Guid entityId = Internal.ECS.Instantiate(entityName);

            return new Entity(entityId);
        }

        /// <summary>
        /// Instantiates an entity with the provided name into the level with a RenderableResource attached
        /// </summary>
        /// <param name="entityName">Name of the entity being instantiated</param>
        /// <param name="modelFileName">Name of the model file to put on the entity</param>
        /// <returns>Reference to the entity instantiated in the level</returns>
        public Entity InstantiateWithModel(string entityName, string modelFileName)
        { 
            Guid entityId = Internal.ECS.InstantiateWithModel(entityName, modelFileName);

            return new Entity(entityId);
        }

        /// <summary>
        /// Instantiates an entity with the provided name into the level with a RenderableResource and a specified position
        /// </summary>
        /// <param name="entityName">Name of the entity being instantiated</param>
        /// <param name="modelFileName">Name of the model file to put on the entity</param>
        /// <param name="position">Reference to the entity instantiated in the level</param>
        /// <returns></returns>
        public Entity InstantiateWithModelWithPosition(string entityName, string modelFileName, Vector3 position)
        { 
            Guid entityId = Internal.ECS.InstantiateWithModelAndPosition(entityName, modelFileName, ref position);

            return new Entity(entityId);
        }

        /// <summary>
        /// Destroy an entity with the provided name in the level
        /// </summary>
        /// <param name="entityName">Name of the entity being destroyed</param>
        public void Destroy(string entityName)
        {
            Internal.ECS.DestroyEntityByName(entityName);
        }

        /// <summary>
        /// Destroy an entity with a provided reference
        /// </summary>
        /// <param name="entity">Reference to the entity being destroyed</param>
        public void Destroy(Entity entity)
        {
            Internal.ECS.DestroyEntityByGuid(entity.GUID);
        }
    }
}
