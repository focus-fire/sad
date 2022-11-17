#pragma once

#include "Resource.h"

namespace sad
{
	class LevelResource final : public Resource
	{
	public:
		LevelResource() = default;
		~LevelResource() = default;

		explicit LevelResource(const Resource::ResourceData& resourceData)
			: Resource(resourceData)
		{ }
	};
}
