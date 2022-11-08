#include "sadpch.h"

#include "Assert.h"

#include <glad/glad.h>

namespace
{
	bool IsGLAlive = false;
}

void OnGLStateChanged()
{
	IsGLAlive = !IsGLAlive;
	core::Log(ELogType::Trace, "OnGLStateChanged turned GL context {}", IsGLAlive ? "on" : "off");
}

void GLClearErrorLog()
{
	if (!IsGLAlive)
		return;

	while (glGetError() != GL_NO_ERROR);
}

bool GLWriteErrorLog(const char* function, const char* file, int line)
{
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