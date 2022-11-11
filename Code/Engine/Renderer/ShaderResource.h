#pragma once

#include <string>
#include <unordered_map>

#include <Engine/Resource.h>

namespace sad::rad
{
	class ShaderResource final : public Resource
	{
	private:
		struct Source
		{
			std::string VertexSource;
			std::string FragmentSource;

			Source(const std::string& vertexSource, const std::string& fragmentSource)
				: VertexSource(vertexSource)
				, FragmentSource(fragmentSource)
			{ }
		};

		enum class Type
		{
			Vertex,
			Fragment,
			None
		};
	
		unsigned int m_RendererId;
		std::string m_FileName;
		std::unordered_map<std::string, int> m_UniformLocations;

	public:
		ShaderResource();
		explicit ShaderResource(const Resource::ResourceData& resourceData, const std::string& combinedShaderPath);

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int i0);
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniformMatrix3fv(const std::string& name, const float* matrix);
		void SetUniformMatrix4fv(const std::string& name, const float* matrix);

	private:
		/**
		 * @brief Retrives the index position of a shader's uniform 
		 * @param name The name of a uniform declared in the shader source
		 * @return Integer representing the uniform's position
		*/
		int GetUniformLocation(const std::string& name);

		/**
		 * @brief Parses a shader file with both fragment and vertex components indicated with appropriate '#shader TYPE' headers 
		 * @param filePath String path to the target shader file
		 * @return Struct containing the string sources for the parsed shader's vertex and fragment components
		*/
		Source ParseCombinedShader(const std::string& filePath);

		/**
		 * @brief Compiles shaders from a source string with shader contents 
		 * @param type GLenum for the type of shader being parsed, normally GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		 * @param shaderSource String containing the parsed source for the target shader
		 * @return Integer representing the ID of the shader created by OpenGL
		*/
		unsigned int CompileShader(unsigned int type, const std::string& shaderSource);

		/**
		 * @brief Creates, compiles, and attaches shader programs - deletes shader sources after compilation 
		 * @param vertexShader Source string for the target vertex shader
		 * @param fragmentShader Source string for the target fragment shader
		 * @return Integer representing the ID of the shader program created by OpenGL 
		*/
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
