#include "sadpch.h"

#include "Assert.h"

#include <glad/glad.h>

void GLClearErrorLog()
{
	// TODO: Replace with GL context event
	if (!IsGLAlive)
		return;

	while (glGetError() != GL_NO_ERROR);
}

bool GLWriteErrorLog(const char* function, const char* file, int line)
{
	// TODO: Replace with GL context event
	if (!IsGLAlive)
		return true;

	GLenum error;

	while ((error = glGetError()))
	{
		core::Log(ELogType::Assert, "OpenGL Error #{} - {} @ {} line {}", error, function, file, line);
		return false;
	}

	return true;
}