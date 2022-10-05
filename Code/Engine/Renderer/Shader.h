#pragma once

#include <string>
#include <unordered_map>

namespace sad::rad
{
	class Shader
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
		std::unordered_map<std::string, int> m_UniformLocations;

	public:
		explicit Shader(const std::string& combinedShaderPath);
		explicit Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int i0);
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniformMatrix3fv(const std::string& name, const float* matrix);
		void SetUniformMatrix4fv(const std::string& name, const float* matrix);

	private:
		int GetUniformLocation(const std::string& name);

		std::string ParseSingleShader(const std::string& filePath);
		Source ParseCombinedShader(const std::string& filePath);

		unsigned int CompileShader(unsigned int type, const std::string& shaderSource);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
