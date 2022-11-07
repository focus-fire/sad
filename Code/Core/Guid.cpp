#include "sadpch.h"

#include "Guid.h"

core::Guid core::Guid::CreateGuid()
{
	core::Guid guid;

#ifdef _SAD_WINDOWS
	UUID uuid;

	RPC_STATUS status = UuidCreate(&uuid);
	SAD_ASSERT(status == RPC_S_OK, "Failed to create Windows GUID");

	guid = Guid(uuid);
#else
	uuid_t uuid;
	uuid_generate_random(uuid);
	SAD_ASSERT(uuid, "Failed to create Unix GUID");

	guid = Guid(uuid);
#endif

	return guid;
}

core::Guid::Guid()
	: m_StringGuid()
{ }

#ifdef _SAD_WINDOWS
core::Guid::Guid(UUID uuid)
	: m_Guid(uuid)
{
	unsigned char* str;
	RPC_STATUS status = UuidToStringA(&uuid, &str);
	SAD_ASSERT(status == RPC_S_OK, "Failed to convert GUID to string");

	m_StringGuid = std::string((char*) str);
	RpcStringFreeA(&str);
}
#else
core::Guid::Guid(uuid_t uuid)
{ 
	uuid_copy(m_Guid, uuid);

	char str[37];
	uuid_unparse_lower(uuid, str);
	SAD_ASSERT(str, "Failed to convert GUID to string");

	m_StringGuid = std::string(str);
}
#endif

int core::Guid::CompareGuid(const Guid& a, const Guid& b)
{
	int result;

	// WARNING: Do NOT do this at home kids
	core::Guid aGuid = const_cast<Guid&>(a);
	core::Guid bGuid = const_cast<Guid&>(b);

#ifdef _SAD_WINDOWS
	RPC_STATUS status;
	result = UuidCompare(&aGuid.m_Guid, &bGuid.m_Guid, &status);
	SAD_ASSERT(status == RPC_S_OK, "Failed to compare GUIDs");
#else 
	result = uuid_compare(a.m_Guid, b.m_Guid);
#endif

	return result;
}
