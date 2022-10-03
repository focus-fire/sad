#include "sadpch.h"

#include "Assert.h"

#include <glad/glad.h>

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLWriteLog(const char* function, const char* file, int line)
{
	GLenum error;

	while ((error = glGetError()))
	{
		core::Log(ELogType::Assert, "OpenGLES Error #{} - {} @ {} line {}", error, function, file, line);
		return false;
	}

	return true;
}