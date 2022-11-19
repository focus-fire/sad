#pragma once

#include <string>

#include <mono/metadata/metadata.h>

#include <Core/Log.h>

#include "ScriptingEngine.h"

namespace sad::cs
{
	class ScriptingBridge
	{
	public:
		struct EntityECSFunctions 
		{
			std::unordered_map<MonoType*, std::function<bool(sad::ecs::Entity&)>> HasComponents;
			std::unordered_map<MonoType*, std::function<void(sad::ecs::Entity&)>> AddComponents;
			std::unordered_map<MonoType*, std::function<void(sad::ecs::Entity&)>> RemoveComponents;
		};

		static EntityECSFunctions s_EntityECSFunctions;

		/**
		 * @brief Initializes the internal calls mono has to make for API functions
		*/
		static void SetupEngineAPIFunctions();

		/**
		 * @brief Initializes and registers all components specified into managed code 
		*/
		static void SetupEngineAPIComponents();

		/**
		 * @brief Registers an unmanaged ECS component for use in C#
		 * @tparam T Struct component to register in C#
		 * @param removeComponentSuffix Marks whether the C# structure/class associated with this component should have a "Component" suffix.
		 *		  For example, 'TransformComponent' is passed but 'Transform' is the class used in C# code.
		*/
		template <typename T>
		static void RegisterManagedComponent(bool removeComponentSuffix = false)
		{
			// Type name in the form 'struct sad::ecs::Component'
			std::string_view type = typeid(T).name();
			size_t finalNamespacePos = type.find_last_of(':');
			std::string_view structName = type.substr(finalNamespacePos + 1);

			// Remove "Component" from the class name in managed types
			// ie: 'TransformComponent' becomes 'Transform' in C#
			std::string componentName = structName.data();
			if (removeComponentSuffix)
			{
				std::string comp = "Component";
				size_t componentStringLocation = structName.find(comp);
				if (componentStringLocation != std::string::npos)
					componentName.erase(componentStringLocation, comp.length());
			}

			// Setup and retrieve the reflected type from the Mono runtime
			std::string managedTypename = fmt::format("Sad.{}", componentName);
			MonoType* reflectedType = mono_reflection_type_from_name(managedTypename.data(), cs::ScriptingEngine::s_ScriptingData->SadCSFrameworkImage);
			if (!reflectedType)
			{
				core::Log(ELogType::Error, "[ScriptingBridge] Failed to find type {} in C# when registering {}", managedTypename, structName);
				return;
			}

			// Register component functions in all three available lookups
			s_EntityECSFunctions.HasComponents[reflectedType] = [](ecs::Entity& entity) { return entity.HasComponent<T>(); };
			s_EntityECSFunctions.AddComponents[reflectedType] = [](ecs::Entity& entity) { entity.AddComponent<T>(); };
			s_EntityECSFunctions.RemoveComponents[reflectedType] = [](ecs::Entity& entity) { entity.RemoveComponent<T>(); };
		}
	};
}
