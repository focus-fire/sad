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
			std::string Name;
			std::string DataPath;
			std::string AbsolutePath;
			core::Guid Guid = core::Guid::CreateGuid();
		};

		Resource();
		Resource(const ResourceData& data);
		virtual ~Resource();

		/**
		 * @brief Serializes the resource in a CSV format used in the Resources.sad.meta file
		 * @return Resource serialized into a string
		*/
		std::string Serialize() const;

		/**
		 * @brief Definition for a null resource
		 * @return Empty resource definition
		*/
		static Resource Null() { return Resource({ "", "", "", core::Guid() }); }

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

		/**
		 * @brief Retrieves the file path relative to the project's `./Data` directory
		 * @return The file path relative to the './Data' directory 
		*/
		inline std::string GetResourceDataPath() const { return m_DataPath; }

		/**
		 * @brief Retrieves the absolute path for the resource on the filesystem
		 * @return The absoluate file path for the resource
		*/
		inline std::string GetResourceAbsolutePath() const { return m_AbsolutePath; }

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
		std::string m_DataPath;
		std::string m_AbsolutePath;
	};
}
