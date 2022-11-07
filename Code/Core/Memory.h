#pragma once

#include <memory>

namespace core 
{
	template<typename T>
	using Pointer = std::shared_ptr<T>;

	template<typename T, typename... TArgs>
	constexpr Pointer<T> CreatePointer(TArgs&& ...args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}
}
