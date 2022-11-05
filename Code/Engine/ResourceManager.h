#pragma once

#include <unordered_map>
#include <unordered_set>

#include <Core/Guid.h>

#include "IResource.h"

namespace sad
{
	/**
	 * @brief Determines the allocation, storage, and management of resources throughout the engine
	*/
	class ResourceManager
	{
	private:
		ResourceManager()
			: m_ResourceMap()
		{ }

		struct ResourceMapEntry 
		{
			core::Guid m_Guid;
			const char* m_Name;
			const char* m_Type;
			const char* m_FilePath;
		};

	private:
		const std::string c_ResourceFilePath = core::FileUtils::GetPathInsideDataDirectory("Resources.sad.meta");
		const std::string c_ResourceFileHeader = "GUID,Name,Type,Path";

		const std::string c_ShaderFileExtensions[2] = { ".glsl", ".shader" };
		const std::string c_TextureFileExtensions[2] = { ".png", ".jpg" };
		const std::string c_RenderableFileExtensions[1] = { ".fbx" };

		std::unordered_map<core::Guid, core::Pointer<IResource>> m_ResourceMap;
		std::unordered_set<std::string> m_ResourceFileSet;

	public:
		ResourceManager(ResourceManager& other) = delete;

		static ResourceManager& GetInstance();

		static void Start();

	private:
		/**
		 * @brief 
		*/
		void MStart();

		void ParseResourceMapCSV(const std::string& csvFileContents);
	};
}
