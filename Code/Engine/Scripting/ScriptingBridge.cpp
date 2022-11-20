#include "sadpch.h"

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

	static ecs::Entity GetEntityInLevel(const core::NativeGuid& guid)
	{
		Level* level = ScriptingEngine::GetCurrentLevelInstance();
		SAD_ASSERT(level, "Failed to retrieve valid level instance");

		ecs::Entity entity = level->GetEntityByGuid(guid);
		SAD_ASSERT(entity, "Failed to retrieve entity from level with passed GUID");

		return entity;
	}

	///////////
	/// ECS ///
	///////////

	static bool HasComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& hasComponentFunctions = ScriptingBridge::s_EntityECSFunctions.HasComponents;
		SAD_ASSERT(hasComponentFunctions.find(componentType) != hasComponentFunctions.end(), "Trying to fetch a component from an enity that doesn't have a registered HasComponent function");

		return hasComponentFunctions[componentType](entity);
	}

	static void AddComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& componentFunctions = ScriptingBridge::s_EntityECSFunctions.AddComponents;
		SAD_ASSERT(componentFunctions.find(componentType) != componentFunctions.end(), "Trying to add a component to an entity that doesn't have a registered AddComponent function");

		std::function<void(ecs::Entity&)> addComponentFunction = componentFunctions[componentType];
		addComponentFunction(entity);
	}

	static void RemoveComponent(core::NativeGuid guid, MonoReflectionType* type)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		MonoType* componentType = mono_reflection_type_get_type(type);

		auto& removeComponentFunctions = ScriptingBridge::s_EntityECSFunctions.RemoveComponents;
		SAD_ASSERT(removeComponentFunctions.find(componentType) != removeComponentFunctions.end(), "Trying to remove a component from an entity that doesn't have a registered RemoveComponent function");

		removeComponentFunctions[componentType](entity);
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

	///////
	///////
	///////

	/////////////////
	/// Transform ///
	/////////////////

	static void GetPosition(core::NativeGuid guid, glm::vec3* outPosition)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		*outPosition = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetPosition();
	}

	static void SetPosition(core::NativeGuid guid, glm::vec3* position)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetPosition(*position);
	} 

	static void GetRotation(core::NativeGuid guid, glm::quat* outRotation)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		*outRotation = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetPosition();
	}

	static void SetRotation(core::NativeGuid guid, glm::quat* rotation)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetRotation(*rotation);
	}

	static void GetScale(core::NativeGuid guid, glm::vec3* outScale)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		*outScale = entity.GetComponent<ecs::TransformComponent>().m_Transform->GetScale();
	}

	static void SetScale(core::NativeGuid guid, glm::vec3* scale)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->SetScale(*scale);
	}

	static void Translate(core::NativeGuid guid, glm::vec3* translation)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Translate(*translation);
	}

	static void Rotate(core::NativeGuid guid, glm::vec3* rotation)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Rotate(*rotation);
	}

	static void RotateByQuaternion(core::NativeGuid guid, glm::quat* rotation)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Rotate(*rotation);
	}

	static void Scale(core::NativeGuid guid, glm::vec3* scale)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		entity.GetComponent<ecs::TransformComponent>().m_Transform->Scale(*scale);
	}

	/////////////
	/// Bound ///
	/////////////

	static void GetBoundMin(core::NativeGuid guid, glm::vec3* outMin)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		*outMin = entity.GetComponent<ecs::BoundComponent>().m_Bound->GetBoundMin();
	}

	static void GetBoundMax(core::NativeGuid guid, glm::vec3* outMax)
	{
		ecs::Entity entity = GetEntityInLevel(guid);
		*outMax = entity.GetComponent<ecs::BoundComponent>().m_Bound->GetBoundMax();
	}
}

void sad::cs::ScriptingBridge::SetupEngineAPIFunctions()
{
	SAD_CSF_ADD_INTERNAL("ECS", HasComponent);
	SAD_CSF_ADD_INTERNAL("ECS", AddComponent);
	SAD_CSF_ADD_INTERNAL("ECS", RemoveComponent);

	SAD_CSF_ADD_INTERNAL("Log", Debug);
	SAD_CSF_ADD_INTERNAL("Log", Warn);
	SAD_CSF_ADD_INTERNAL("Log", Error);

	SAD_CSF_ADD_INTERNAL("Gizmos", DrawLine);
	SAD_CSF_ADD_INTERNAL("Gizmos", DrawBox);

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

	SAD_CSF_ADD_INTERNAL("Bound", GetBoundMin);
	SAD_CSF_ADD_INTERNAL("Bound", GetBoundMax);
}

void sad::cs::ScriptingBridge::SetupEngineAPIComponents()
{
	RegisterManagedComponent<ecs::TransformComponent>(true);
	RegisterManagedComponent<ecs::BoundComponent>(true);
}
