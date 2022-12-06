#include "sadpch.h"

// Import engines stuff
#include <Game/Time.h>
#include <Engine/Application.h>
#include <Engine/AudioManager.h>
#include <Engine/ResourceManager.h>
#include <Engine/InputManager.h>
#include <Engine/Raycast.h>

#include "ScriptingBridge.h"

#include <glm/glm.hpp>
#include <mono/metadata/object.h>

#include <Engine/ECS/Components/ComponentTypes.h>
#include <Engine/Renderer/RenderBuddy.h>

/// Refer to SadCSFramework.Internal.cs for a reference on currently implemented internal methods
/// In order to add a new method to the scripting api, use this macro with the className (in the 'Sad.Internal' namespace) and the method name
/// Note: Parameters can be added to the 'mono_add_internal_call' signature in order to define an overload
/// ie: mono_add_internal_call("Sad.Internal.Log::Debug(string)") vs mono_add_internal_call("Sad.Internal.Log::Debug(Vector3)")
#define SAD_CSF_ADD_INTERNAL(className, method) mono_add_internal_call("Sad.Internal." className "::" #method, (const void*) method)

sad::cs::ScriptingBridge::EntityECSFunctions sad::cs::ScriptingBridge::s_EntityECSFunctions;

namespace sad::cs
{
	////////////
	/// Util ///
	////////////

	static ecs::Entity GetEntityInLevelByGUID(const core::NativeGuid& guid)
	{
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		SAD_ASSERT(level, "Failed to retrieve valid level instance");

		ecs::Entity entity = level->GetEntityByGuid(guid);
		SAD_ASSERT(entity, "Failed to retrieve entity from level with passed GUID");

		return entity;
	}

	static ecs::Entity GetEntityInLevelByName(const std::string& entityName)
	{
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		SAD_ASSERT(level, "Failed to retrieve valid level instance");

		ecs::Entity entity = level->GetEntityByName(entityName);
		SAD_ASSERT(entity, "Failed to retrieve entity from level with passed name");

		return entity;
	}

	///////////
	/// ECS ///
	///////////

	static core::NativeGuid FindEntityByName(MonoString* entityName)
	{
		char* cString = mono_string_to_utf8(entityName);

		ecs::Entity entity = GetEntityInLevelByName(cString);

		mono_free(cString);

		// TODO: What can be reliably returned here?
		//		 This is currently a bit of a nuclear bomb...
		if (!entity) 
			return core::NativeGuid();

		return entity.GetGuid().GetNativeGuid();
	}

	static core::NativeGuid Instantiate(MonoString* entityName)
	{
		char* cString = mono_string_to_utf8(entityName);

		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		ecs::Entity newEntity = level->InstantiateEntity(cString);

		mono_free(cString);

		return newEntity.GetGuid().GetNativeGuid();
	}

	static core::NativeGuid InstantiateWithModel(MonoString* entityName, MonoString* resourceName)
	{
		char* entityString = mono_string_to_utf8(entityName);
		char* resourceString = mono_string_to_utf8(resourceName);
		
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		ecs::Entity newEntity = level->InstantiateEntity(entityString);

		ModelResource* modelResource = ResourceManager::GetResource<ModelResource>(resourceString);
		SAD_ASSERT(modelResource, "Attempting to instantiate an entity with a model that hasn't been cached by the ResourceManager");

		// Create the model resource and mark it as dirty
		ecs::ModelResourceComponent modelResourceComponent;
		modelResourceComponent.m_Model = modelResource;
		modelResourceComponent.m_IsResourceDirty = true;
		newEntity.AddComponent<ecs::ModelResourceComponent>(modelResourceComponent);

		mono_free(entityString);
		mono_free(resourceString);

		return newEntity.GetGuid().GetNativeGuid();
	}

	static core::NativeGuid InstantiateWithModelAndPosition(MonoString* entityName, MonoString* resourceName, glm::vec3* position)
	{
		char* entityString = mono_string_to_utf8(entityName);
		char* resourceString = mono_string_to_utf8(resourceName);
		
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		ecs::Entity newEntity = level->InstantiateEntity(entityString);

		ModelResource* modelResource = ResourceManager::GetResource<ModelResource>(resourceString);
		SAD_ASSERT(modelResource, "Attempting to instantiate an entity with a model that hasn't been cached by the ResourceManager");

		// Move the entity to the specified position
		Transform* transform = newEntity.GetComponent<ecs::TransformComponent>().m_Transform.get();
		transform->SetPosition(*position);

		// Create the model resource and mark it as dirty
		ecs::ModelResourceComponent modelResourceComponent;
		modelResourceComponent.m_Model = modelResource;
		modelResourceComponent.m_IsResourceDirty = true;
		newEntity.AddComponent<ecs::ModelResourceComponent>(modelResourceComponent);

		mono_free(entityString);
		mono_free(resourceString);

		return newEntity.GetGuid().GetNativeGuid();
	}

	static void DestroyEntityByName(MonoString* entityName)
	{
		char* entityString = mono_string_to_utf8(entityName);

		ScriptingEngine::GetCurrentLevelInstance()->DestroyEntityByName(entityString);

		mono_free(entityString);
	}

	static void DestroyEntityByGuid(core::NativeGuid guid)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		ScriptingEngine::GetCurrentLevelInstance()->DestroyEntity(entity);
	}

	static bool HasComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& hasComponentFunctions = ScriptingBridge::s_EntityECSFunctions.HasComponents;
		SAD_ASSERT(hasComponentFunctions.find(componentType) != hasComponentFunctions.end(), "Trying to fetch a component from an enity that doesn't have a registered HasComponent function");

		return hasComponentFunctions[componentType](entity);
	}

	static void AddComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& componentFunctions = ScriptingBridge::s_EntityECSFunctions.AddComponents;
		SAD_ASSERT(componentFunctions.find(componentType) != componentFunctions.end(), "Trying to add a component to an entity that doesn't have a registered AddComponent function");

		std::function<void(ecs::Entity&)> addComponentFunction = componentFunctions[componentType];
		addComponentFunction(entity);
	}

	static void RemoveComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& removeComponentFunctions = ScriptingBridge::s_EntityECSFunctions.RemoveComponents;
		SAD_ASSERT(removeComponentFunctions.find(componentType) != removeComponentFunctions.end(), "Trying to remove a component from an entity that doesn't have a registered RemoveComponent function");

		removeComponentFunctions[componentType](entity);
	}

	static bool HasScriptInstance(core::NativeGuid guid)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		return ScriptingEngine::SadBehaviourInstanceExists(entity.GetGuid());
	}

	static MonoObject* GetScriptInstance(core::NativeGuid guid)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		return ScriptingEngine::GetSadBehaviourInstance(entity.GetGuid());
	}

	static void AddScriptInstance(core::NativeGuid guid, MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		ecs::Entity entity = GetEntityInLevelByGUID(guid);

		ScriptingEngine::AddRuntimeSadBehaviourInstance(entity, cString);

		mono_free(cString);
	}

	static void RemoveScriptInstance(core::NativeGuid guid)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		ScriptingEngine::DestroySadBehaviourInstance(entity);
	}

	///////////
	/// Log ///
	///////////

	static void Info(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Info, "{}", cString);

		mono_free(cString);
	}

	static void Debug(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Debug, "{}", cString);

		mono_free(cString);
	}

	static void Warn(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Warn, "{}", cString);

		mono_free(cString);
	}

	static void Error(MonoString* string)
	{
		char* cString = mono_string_to_utf8(string);

		core::Log(ELogType::Error, "{}", cString);

		mono_free(cString);
	}
	
	//////////////
	/// Gizmos ///
	//////////////

	static void DrawLine(glm::vec3* from, glm::vec3* to, glm::vec4* color)
	{
		rad::RenderBuddy::DrawDebugLine(*from, *to, *color);
	}

	static void DrawBox(glm::vec3* min, glm::vec3* max, glm::vec4* color)
	{
		rad::RenderBuddy::DrawDebugBox(*min, *max, *color);
	}

	/////////////////
	/// Transform ///
	/////////////////

	static void GetPosition(core::NativeGuid guid, glm::vec3* outPosition)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		*outPosition = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetPosition();
	}

	static void SetPosition(core::NativeGuid guid, glm::vec3* position)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetPosition(*position);
	} 

	static void GetRotation(core::NativeGuid guid, glm::quat* outRotation)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		*outRotation = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetRotation();
	}

	static void SetRotation(core::NativeGuid guid, glm::quat* rotation)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetRotation(*rotation);
	}

	static void GetScale(core::NativeGuid guid, glm::vec3* outScale)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		*outScale = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetScale();
	}

	static void SetScale(core::NativeGuid guid, glm::vec3* scale)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetScale(*scale);
	}

	static void Translate(core::NativeGuid guid, glm::vec3* translation)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Translate(*translation);
	}

	static void Rotate(core::NativeGuid guid, glm::vec3* rotation)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Rotate(*rotation);
	}

	static void RotateByQuaternion(core::NativeGuid guid, glm::quat* rotation)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Rotate(*rotation);
	}

	static void Scale(core::NativeGuid guid, glm::vec3* scale)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Scale(*scale);
	}

	static glm::quat Slerp(glm::quat quatStart, glm::quat quatEnd, float interpFactor)
	{
		return glm::slerp(quatStart, quatEnd, interpFactor);
	}

	static glm::quat LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	{
		return glm::lookAt(eye, center, up);
	}

	/////////////
	/// Bound ///
	/////////////

	static void GetBoundMin(core::NativeGuid guid, glm::vec3* outMin)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		*outMin = entity.GetComponent<ecs::BoundComponent>().m_Bound->GetBoundMin();
	}

	static void GetBoundMax(core::NativeGuid guid, glm::vec3* outMax)
	{
		ecs::Entity entity = GetEntityInLevelByGUID(guid);
		*outMax = entity.GetComponent<ecs::BoundComponent>().m_Bound->GetBoundMax();
	}

	/////////////
	/// Input ///
	/////////////

	static bool GetButton(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButton(button);
	}

	static bool GetButtonPressed(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButtonPressed(button);
	}

	static bool GetButtonReleased(sad::ControllerButton button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetButtonReleased(button);
	}

	static bool GetKey(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKey(key);
	}

	static bool GetKeyPressed(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKeyPressed(key);
	}

	static bool GetKeyReleased(sad::KeyCode key)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetKeyReleased(key);
	}

	static bool GetMouseButton(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButton(button);
	}

	static bool GetMouseButtonPressed(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButtonPressed(button);
	}

	static bool GetMouseButtonReleased(int button)
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMouseButtonReleased(button);
	}

	static SDL_Point GetMousePosition()
	{
		InputManager& input = InputManager::GetInstance();

		return input.GetMousePosition();
	}

	static bool IsControllerActive()
	{
		InputManager& input = InputManager::GetInstance();

		return input.IsControllerActive();
	}

	static bool IsControllerConnected()
	{
		InputManager& input = InputManager::GetInstance();

		return input.IsControllerConnected();
	}

	/////////////
	/// Audio ///
	/////////////

	static void PlaySFX(MonoString* audioString)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::PlaySFX(audio);

		mono_free(cAudioString);
	}

	static void PlayMusic(MonoString* audioString, int times)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::PlayMusic(audio, times);

		mono_free(cAudioString);
	}

	static void SetVolume(MonoString* audioString, int volume)
	{
		char* cAudioString = mono_string_to_utf8(audioString);

		sad::AudioResource* audio = sad::ResourceManager::GetResource<sad::AudioResource>(cAudioString);

		sad::AudioManager::SetVolume(audio, volume);

		mono_free(cAudioString);
	}

	////////////
	/// Misc ///
	////////////

	static float Getdt()
	{
		return sad::Application::s_DeltaTime;
	}

	static bool RaycastIntersection(glm::vec3* origin, glm::vec3* direction, glm::vec3* position, glm::vec3* size)
	{
		Bound bound = Bound(*position, *size);
		return sad::Raycast::Intersects(*origin, *direction, bound);
	}
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	// ECS
	SAD_CSF_ADD_INTERNAL("ECS", FindEntityByName);
	SAD_CSF_ADD_INTERNAL("ECS", Instantiate);
	SAD_CSF_ADD_INTERNAL("ECS", InstantiateWithModel);
	SAD_CSF_ADD_INTERNAL("ECS", InstantiateWithModelAndPosition);
	SAD_CSF_ADD_INTERNAL("ECS", DestroyEntityByName);
	SAD_CSF_ADD_INTERNAL("ECS", DestroyEntityByGuid);

	SAD_CSF_ADD_INTERNAL("ECS", HasComponent);
	SAD_CSF_ADD_INTERNAL("ECS", AddComponent);
	SAD_CSF_ADD_INTERNAL("ECS", RemoveComponent);

	SAD_CSF_ADD_INTERNAL("ECS", HasScriptInstance);
	SAD_CSF_ADD_INTERNAL("ECS", GetScriptInstance);
	SAD_CSF_ADD_INTERNAL("ECS", AddScriptInstance);
	SAD_CSF_ADD_INTERNAL("ECS", RemoveScriptInstance);

	// Log
	SAD_CSF_ADD_INTERNAL("Log", Debug);
	SAD_CSF_ADD_INTERNAL("Log", Warn);
	SAD_CSF_ADD_INTERNAL("Log", Error);

	// Gizmos
	SAD_CSF_ADD_INTERNAL("Gizmos", DrawLine);
	SAD_CSF_ADD_INTERNAL("Gizmos", DrawBox);

	// Transform
	SAD_CSF_ADD_INTERNAL("Transform", GetPosition);
	SAD_CSF_ADD_INTERNAL("Transform", SetPosition);
	SAD_CSF_ADD_INTERNAL("Transform", GetRotation);
	SAD_CSF_ADD_INTERNAL("Transform", SetRotation);
	SAD_CSF_ADD_INTERNAL("Transform", GetScale);
	SAD_CSF_ADD_INTERNAL("Transform", SetScale);
	SAD_CSF_ADD_INTERNAL("Transform", Translate);
	SAD_CSF_ADD_INTERNAL("Transform", Rotate);
	SAD_CSF_ADD_INTERNAL("Transform", RotateByQuaternion);
	SAD_CSF_ADD_INTERNAL("Transform", Scale);
	SAD_CSF_ADD_INTERNAL("Transform", Slerp);
	SAD_CSF_ADD_INTERNAL("Transform", LookAt);

	// Bound
	SAD_CSF_ADD_INTERNAL("Bound", GetBoundMin);
	SAD_CSF_ADD_INTERNAL("Bound", GetBoundMax);

	// Inputs
	SAD_CSF_ADD_INTERNAL("Input", GetButton);
	SAD_CSF_ADD_INTERNAL("Input", GetButtonPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetButtonReleased);
	SAD_CSF_ADD_INTERNAL("Input", GetKey);
	SAD_CSF_ADD_INTERNAL("Input", GetKeyPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetKeyReleased);
	SAD_CSF_ADD_INTERNAL("Input", GetMouseButton);
	SAD_CSF_ADD_INTERNAL("Input", GetMouseButtonPressed);
	SAD_CSF_ADD_INTERNAL("Input", GetMouseButtonReleased);
	SAD_CSF_ADD_INTERNAL("Input", GetMousePosition);
	SAD_CSF_ADD_INTERNAL("Input", IsControllerActive);
	SAD_CSF_ADD_INTERNAL("Input", IsControllerConnected);
	SAD_CSF_ADD_INTERNAL("Input", IsControllerConnected);

	// Audio
	SAD_CSF_ADD_INTERNAL("Audio", PlaySFX);
	SAD_CSF_ADD_INTERNAL("Audio", PlayMusic);
	SAD_CSF_ADD_INTERNAL("Audio", SetVolume);

	// Misc
	SAD_CSF_ADD_INTERNAL("Misc", Getdt);
	SAD_CSF_ADD_INTERNAL("Misc", RaycastIntersection);
}

void sad::cs::ScriptingBridge::SetupEngineAPIComponents()
{
	RegisterManagedComponent<ecs::TransformComponent>(true);
	RegisterManagedComponent<ecs::BoundComponent>(true);
}
