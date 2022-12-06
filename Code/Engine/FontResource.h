#pragma once

#include "Resource.h"

namespace sad
{
	class FontResource final : public Resource
	{
	public:
		FontResource()
			: Resource(Resource::Null())
		{ }

		explicit FontResource(const Resource::ResourceData& resourceData)
			: Resource(resourceData)
		{ }

		~FontResource() = default;
	};
}
