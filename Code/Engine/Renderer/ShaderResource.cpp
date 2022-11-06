#include "sadpch.h"

#include "ShaderResource.h"

#include <fstream>
#include <sstream>

#include <glad/glad.h>

sad::rad::ShaderResource::ShaderResource(const IResource::ResourceData& resourceData, const std::string& combinedShaderPath)
    : IResource(resourceData)
    , m_FileName(combinedShaderPath)
{
    const Source source = ParseCombinedShader(combinedShaderPath);

    m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
    SAD_ASSERT(m_RendererId, "Shader was properly created in the renderer")
    core::Log(ELogType::Trace, "[ShaderResource] Shader program object created for shader with ID #{}", m_RendererId);

    GL_CALL(glUseProgram(m_RendererId));
}

void sad::rad::ShaderResource::Bind() const
{
    GL_CALL(glUseProgram(m_RendererId));
}

void sad::rad::ShaderResource::Unbind() const
{
    GL_CALL(glUseProgram(0));
}

int sad::rad::ShaderResource::GetUniformLocation(const std::string& name)
{
    // Return uniform location if it exists in the cache
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations[name];

    // Check if uniform exists and add it to cache
    GL_CALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));

    if (location == -1)
        core::Log(ELogType::Warn, "[ShaderResource] Uniform {} not found on shader with ID#{}", name, m_RendererId);

    m_UniformLocations[name] = location;

    return location;
}

void sad::rad::ShaderResource::SetUniform1i(const std::string& name, int i0)
{
    GL_CALL(glUniform1i(GetUniformLocation(name), i0));
}

void sad::rad::ShaderResource::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void sad::rad::ShaderResource::SetUniformMatrix3fv(const std::string& name, const float* matrix)
{
    GL_CALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

void sad::rad::ShaderResource::SetUniformMatrix4fv(const std::string& name, const float* matrix)
{
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

sad::rad::ShaderResource::Source sad::rad::ShaderResource::ParseCombinedShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    Type type = ShaderResource::Type::None;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderResource::Type::Vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderResource::Type::Fragment;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int sad::rad::ShaderResource::CompileShader(unsigned int type, const std::string& shaderSource)
{
    unsigned int shaderId = glCreateShader(type);
    const char* src = shaderSource.c_str();

    GL_CALL(glShaderSource(shaderId, 1, &src, nullptr));
    GL_CALL(glCompileShader(shaderId));

    int ivResult;
    GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult));

    if (ivResult == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

        char* errorLog = (char*) malloc(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shaderId, length, &length, errorLog));

        core::Log(ELogType::Error, "[ShaderResource] Failed to compile {} shader", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        core::Log(ELogType::Error, "[ShaderResource] {}", errorLog);

        GL_CALL(glDeleteShader(shaderId));

        return 0;
    }

    return shaderId;
}

unsigned int sad::rad::ShaderResource::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int glProgram = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    // Attach, Link, and Validate shader program
    GL_CALL(glAttachShader(glProgram, vs));
    GL_CALL(glAttachShader(glProgram, fs));
    GL_CALL(glLinkProgram(glProgram));
    GL_CALL(glValidateProgram(glProgram));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return glProgram;
}
