#pragma once

#include "Define.h"

#ifdef _SAD_ENABLE_ASSERTS
	/*
	 * @brief Core engine assert that creates a debug breakpoint and logs a message if a conditional fails
	 * @param check Condition to validate when the assert is called
	 * @param message String message logged to the assert channel if the assert is triggered
	*/
	#define SAD_ASSERT(check, message) { if(!(check)) { core::Log(ELogType::Assert, "{} @ {} line {}", message, SAD_FILE_NAME, __LINE__); SAD_DEBUGBREAK(); } }

	/*
	 * @brief Lazy engine assert that prints the condition supplied instead of displaying a custom message
	 * @param check Condition to validate when the assert is called
	*/
	#define SAD_LAZY_ASSERT(check) { SAD_EXPAND_MACRO( SAD_ASSERT(check, SAD_STRINGIFY_MACRO(check)) ) }
#else
	#define SAD_ASSERT(check, message)
	#define SAD_LAZY_ASSERT(check)
#endif

/*
 * @brief  Nasty hack to prevent logs from spamming on application teardown
 *		   Essentially, GL will unbind all active elements and free any GL-required memory when context is killed
 *		   However, entt will kill any existing components when it dies and in turn will kill GL elements that make GL calls
 *		   This shouldn't cause memory leak - but it'll make endless loops of GL errors that we don't care about (entt is freeing its memory and GL already did)
 */
static bool IsGLAlive = false;

/**
 * @brief Clears the GL error buffer 
*/
void GLClearErrorLog();

/**
 * @brief Checks for any errors in the GL error buffer and logs them to the Error channel
 * @param function The function call from which the error was caused
 * @param file String containing the name for the file from which the error was caused
 * @param line Line number in the file from which the error was caused
 * @return True if no errors were logged to the console
*/
bool GLWriteErrorLog(const char* function, const char* file, int line);

#define GL_CALL(x) GLClearErrorLog(); x;\
	SAD_ASSERT(GLWriteErrorLog(#x, SAD_FILE_NAME_C_STR, __LINE__), "GL_CALL failed, refer to OpenGL trace above for more information");
