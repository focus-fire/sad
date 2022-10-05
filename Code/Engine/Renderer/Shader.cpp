#include "sadpch.h"

#include "Shader.h"

#include <fstream>

#include <glad/glad.h>

sad::rad::Shader::Shader(const std::string& combinedShaderPath)
{
    const Source source = ParseCombinedShader(combinedShaderPath);

    m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
    SAD_ASSERT(m_RendererId, "Shader was properly created in the renderer")
    core::Log(ELogType::Info, "Shader program object created for shader with Id #{}", m_RendererId);

    GL_CALL(glUseProgram(m_RendererId));
}

sad::rad::Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    const std::string vertexSource = ParseSingleShader(vertexFilePath);
    const std::string fragmentSource = ParseSingleShader(fragmentFilePath);
    const Source source = Source(vertexSource, fragmentSource);

    m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
    SAD_ASSERT(m_RendererId, "Shader was properly created in the renderer")
    core::Log(ELogType::Info, "Shader program object created for shader with Id #{}", m_RendererId);

    GL_CALL(glUseProgram(m_RendererId));
}

void sad::rad::Shader::Bind() const
{
    GL_CALL(glUseProgram(m_RendererId));
}

void sad::rad::Shader::Unbind() const
{
    GL_CALL(glUseProgram(0));
}

/// <summary>
/// Retrives the index position of a shader's uniform  
/// </summary>
/// <param name="name">The name of a uniform declared in the shader source</param>
/// <returns>Integer representing the uniform's position</returns>
int sad::rad::Shader::GetUniformLocation(const std::string& name)
{
    // Return uniform location if it exists in the cache
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations[name];

    // Check if uniform exists and add it to cache
    GL_CALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));

    if (location == -1)
        core::Log(ELogType::Warn, "Uniform {} not found on shader with ID#{}", name, m_RendererId);

    m_UniformLocations[name] = location;

    return location;
}

void sad::rad::Shader::SetUniform1i(const std::string& name, int i0)
{
    GL_CALL(glUniform1i(GetUniformLocation(name), i0));
}

void sad::rad::Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void sad::rad::Shader::SetUniformMatrix3fv(const std::string& name, const float* matrix)
{
    GL_CALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

void sad::rad::Shader::SetUniformMatrix4fv(const std::string& name, const float* matrix)
{
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

/// <summary>
/// Parses a single shader with either a fragment or a vertex component denoted by a '#shader TYPE' header
/// </summary>
std::string sad::rad::Shader::ParseSingleShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                continue;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                continue;
            }
        }
        else
        {
            ss << line << "\n";
        }
    }

    return ss.str();
}

/// <summary>
/// Parses a shader file with both fragment and vertex components indicated with appropriate '#shader TYPE' headers 
/// </summary>
sad::rad::Shader::Source sad::rad::Shader::ParseCombinedShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    Type type = Shader::Type::None;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = Shader::Type::Vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = Shader::Type::Fragment;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/// <summary>
/// Compiles shaders from a source string with shader contents 
/// </summary>
unsigned int sad::rad::Shader::CompileShader(unsigned int type, const std::string& shaderSource)
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

        core::Log(ELogType::Error, "Failed to compile {} shader", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        core::Log(ELogType::Error, "{}", errorLog);

        GL_CALL(glDeleteShader(shaderId));

        return 0;
    }

    return shaderId;
}

/// <summary>
/// Creates, compiles, and attaches shader programs and deletes shader sources after compilation
/// </summary>
unsigned int sad::rad::Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
