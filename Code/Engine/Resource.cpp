#include "sadpch.h"

#include "Resource.h"

sad::Resource::Resource()
{ }

sad::Resource::~Resource()
{ }

sad::Resource::Resource(const ResourceData& data)
	: m_Name(data.Name)
	, m_DataPath(data.DataPath)
	, m_AbsolutePath(data.AbsolutePath)
	, m_Guid(data.Guid)
{ }

std::string sad::Resource::Serialize() const
{
	return m_Guid.ToString() + "," + m_Name + "," + m_DataPath;
}
