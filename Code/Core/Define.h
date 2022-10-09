#pragma once

#include <filesystem>

#ifdef _SAD_DEBUG
	#if defined(_SAD_WINDOWS)
		#define SAD_DEBUGBREAK() __debugbreak() 
	#elif defined(_SAD_MAC)
		#include <signal.h>
		#define SAD_DEBUGBREAK() raise(SIGTRAP)
	#endif
#else
	#define SAD_DEBUGBREAK()
#endif

#define SAD_EXPAND_MACRO(x) x
#define SAD_STRINGIFY_MACRO(x) #x

#define SAD_FILE_NAME std::filesystem::path(__FILE__).filename().string()
#define SAD_FILE_NAME_C_STR std::filesystem::path(__FILE__).filename().string().c_str()

#define INT_TO_VOIDP(i) (void*)(uintptr_t)(i)
