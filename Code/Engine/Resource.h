#pragma once

#include <Core/Log.h>
#include <Core/Guid.h>

namespace sad
{
	/**
	 * @brief Main container for engine resources - these include models, textures, audio, and more
	*/
	class Resource
	{
	public:
		/**
		 * @brief Holds data common to the serialization and deserialization of all resource types
		*/
		struct ResourceData
		{
			core::Guid Guid;
			std::string Name;
			std::string FilePath;

			ResourceData(const std::string& name, const std::string& filePath, core::Guid guid = core::Guid::CreateGuid());
		};

		Resource();
		Resource(const ResourceData& data);
		virtual ~Resource() { }

		/**
		 * @brief Serializes the resource in a CSV format used in the Resources.sad.meta file
		 * @return Resource serialized into a string
		*/
		std::string Serialize() const;

		/**
		 * @brief Definition for a null resource
		 * @return Empty resource definition
		*/
		static Resource Null() { return Resource({ "", "", core::Guid() }); }

		/**
		 * @brief Retrieves the GUID associated with the called resource
		 * @return Native GUID identifying the resource
		*/
		inline core::Guid GetResourceGuid() const { return m_Guid; }

		/**
		 * @brief Retrieves the file name associated with the called resource 
		 * @return String representing the file name for the resource
		*/
		inline std::string GetResourceFileName() const { return m_Name; }

		// TODO: Add relative/absolute path distinction?
		inline std::string GetResourceFilePath() const { return m_FilePath; }

		bool operator==(const Resource& other) const
		{
			return m_Guid == other.m_Guid;
		}

		bool operator!=(const Resource& other) const
		{
			return m_Guid != other.m_Guid;
		}

	private:
		core::Guid m_Guid;
		std::string m_Name;
		std::string m_FilePath;
	};
}
