#pragma once

#include <entt/entt.hpp>

namespace sad::ecs
{
	typedef entt::registry EntityWorld;

	class Registry
	{
	private:
		Registry() { }

	public:
		Registry(const Registry&) = delete;

		static Registry* GetInstance();

		static EntityWorld* GetEntityWorld();

		static entt::entity GenerateEntityHandle();

		EntityWorld World;
	};
}
