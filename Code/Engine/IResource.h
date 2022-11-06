#pragma once

#include <Core/Log.h>
#include <Core/Guid.h>

namespace sad
{
	class IResource
	{
	public:
		explicit IResource(std::string name, std::string filePath, core::Guid guid = core::Guid::CreateGuid())
				: m_Guid(guid)
				, m_Name(std::move(name))
				, m_FilePath(std::move(filePath))
		{
			core::Log(ELogType::Trace, "Created resource for {} with GUID {}", m_Name, m_Guid.ToString());
		}

		virtual ~IResource() { }

		inline core::Guid GetResourceId() const { return m_Guid; }
		inline std::string GetResourceName() const { return m_Name; }
		inline std::string GetResourceFilePath() const { return m_FilePath; }

		inline std::string Serialize() const
		{
			return m_Guid.ToString() + "," + m_Name + "," + m_FilePath;
 		}

	private:
		core::Guid m_Guid;
		std::string m_Name;
		std::string m_FilePath;
	};
}
