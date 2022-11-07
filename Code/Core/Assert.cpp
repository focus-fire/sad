#include "sadpch.h"

#include "Assert.h"

#include <glad/glad.h>

void GLClearErrorLog()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLWriteErrorLog(const char* function, const char* file, int line)
{
	GLenum error;

	while ((error = glGetError()))
	{
		core::Log(ELogType::Assert, "[GLError] OpenGL Error #{} - {} @ {} line {}", error, function, file, line);
		return false;
	}

	return true;
}