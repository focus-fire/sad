#include "sadpch.h"

#include "Guid.h"

core::Guid core::Guid::CreateGuid()
{
	core::Guid guid;

#ifdef _SAD_WINDOWS
	GUID wGuid;

	RPC_STATUS status = UuidCreate(&wGuid);
	SAD_ASSERT(status == RPC_S_OK, "Failed to create Windows GUID");

	guid = Guid(wGuid);
#else
	uuid_t uuid;
	uuid_generate_random(uuid);
	SAD_ASSERT(uuid, "Failed to create Unix GUID");

	guid = Guid(uuid);
#endif

	return guid;
}

core::Guid core::Guid::RecreateGuid(const std::string& stringGuid)
{
	core::Guid guid;

#ifdef _SAD_WINDOWS
	// Completely hacky - really wish this could be avoided at the moment
	auto uStringGuid = reinterpret_cast<unsigned char*>(const_cast<char*>(stringGuid.c_str()));

	GUID wGuid;

	RPC_STATUS status = UuidFromStringA(uStringGuid, &wGuid);
	SAD_ASSERT(status == RPC_S_OK, "Failed to recreate Windows GUID");

	guid = Guid(wGuid);
#else
	uuid_t uuid;
	int result = uuid_parse(stringGuid.c_str(), uuid);
	SAD_ASSERT(result == 0, "Failed to recreate Unix GUID");

	guid = Guid(uuid);
#endif

	return guid;
}

core::Guid::Guid()
	: m_StringGuid()
{ }

core::Guid::Guid(NativeGuid guid)
	: m_Guid(guid)
{
#ifdef _SAD_WINDOWS
	unsigned char* str;
	RPC_STATUS status = UuidToStringA(&guid, &str);
	SAD_ASSERT(status == RPC_S_OK, "Failed to convert GUID to string");

	m_StringGuid = std::string((char*) str);
	RpcStringFreeA(&str);
#else
	uuid_copy(m_Guid, guid);

	char str[37];
	uuid_unparse_lower(guid, str);
	SAD_ASSERT(str, "Failed to convert GUID to string");

	m_StringGuid = std::string(str);
#endif
}

int core::Guid::CompareGuid(const Guid& a, const Guid& b)
{
	int result;

#ifdef _SAD_WINDOWS
	// WARNING: Do NOT do this at home kids
	core::Guid aGuid = const_cast<Guid&>(a);
	core::Guid bGuid = const_cast<Guid&>(b);

	RPC_STATUS status;
	result = UuidCompare(&aGuid.m_Guid, &bGuid.m_Guid, &status);
	SAD_ASSERT(status == RPC_S_OK, "Failed to compare GUIDs");
#else 

	// Note: These have to be reconstructed from unsigned char* to uuid_t on Mac.
	//	     Potentially worth further investigation, but this seems to be the only fix atm.
	uuid_t aUUID, bUUID;
	int aResult = uuid_parse(a.ToString().c_str(), aUUID);
	int bResult = uuid_parse(b.ToString().c_str(), bUUID);
	SAD_ASSERT(aResult == 0, "Failed to recreate Unix GUID");
	SAD_ASSERT(bResult == 0, "Failed to recreate Unix GUID");

	result = uuid_compare(aUUID, bUUID);
#endif

	return result;
}
