#pragma once

#include <Core/Log.h>
#include <Core/Guid.h>

namespace sad
{
	class IResource
	{
	public:
		explicit IResource(std::string name)
			: m_Name(std::move(name))
		{
			m_Guid = core::Guid::CreateGuid();
			core::Log(ELogType::Trace, "Created IResource with GUID {}", m_Guid.ToString());
		}

		virtual ~IResource() { }

		inline core::Guid GetResourceId() const { return m_Guid; }
		inline std::string GetResourceName() const { return m_Name; }

	private:
		core::Guid m_Guid;
		std::string m_Name;
	};
}
