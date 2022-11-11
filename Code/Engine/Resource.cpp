#include "sadpch.h"

#include "Resource.h"

sad::Resource::ResourceData::ResourceData(const std::string& name, 
	const std::string& filePath, 
	core::Guid guid /* = core::Guid() */)
	: Guid(guid)
	, Name(name)
	, FilePath(filePath)
{ }

sad::Resource::Resource()
{ }

sad::Resource::Resource(const ResourceData& data)
	: m_Guid(data.Guid)
	, m_Name(data.Name)
	, m_FilePath(data.FilePath)
{ }

std::string sad::Resource::Serialize() const
{
	return m_Guid.ToString() + "," + m_Name + "," + m_FilePath;
}
