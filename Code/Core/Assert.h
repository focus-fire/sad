#pragma once

#include "Define.h"

// Core Asserts

#ifdef _SAD_ENABLE_ASSERTS
	#define SAD_ASSERT(check, message) { if(!(check)) { core::Log(ELogType::Assert, "{} @ {} line {}", message, SAD_FILE_NAME, __LINE__); SAD_DEBUGBREAK(); } }
	#define SAD_LAZY_ASSERT(check) { SAD_EXPAND_MACRO( SAD_ASSERT(check, SAD_STRINGIFY_MACRO(check)) ) }
#else
	#define SAD_ASSERT(check, message)
	#define SAD_LAZY_ASSERT(check)
#endif

// GL Asserts

void GLClearErrors();
bool GLWriteLog(const char* function, const char* file, int line);

#define GL_CALL(x) GLClearErrors();\
	x;\
	SAD_ASSERT(GLWriteLog(#x, SAD_FILE_NAME_C_STR, __LINE__), "Error checking on a GL_CALL failed. Refer to OpenGL trace for more information.");
