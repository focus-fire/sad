#pragma once

extern "C"
{
#ifdef _SAD_WINDOWS
#include <rpc.h>
#include <rpcdce.h>
#else
#include <uuid/uuid.h>
#endif
}

#include <string>

#include <Core/Assert.h>

namespace core
{
	/**
	 * @brief Globally Unique Identifier, often used to identify things
	*/
	class Guid
	{
	public:
		/**
		 * @brief Factory method to generate platform-specific GUIDs
		 * @return Properly constructed platform specific GUID
		*/
		static Guid CreateGuid();

		/**
		 * @brief Retrieves string representation for the GUID 
		 * @return String representing the GUID
		*/
		inline std::string ToString() const { return m_StringGuid; }

		/**
		 * @brief Evaluates if a generated GUID is null
		 * @return True if the GUID is null, false if has a GUID 
		*/
		inline bool IsNull() const { return m_StringGuid.empty(); }

		/**
		 * @brief Compares current Guid against another specified Guid for equality using private comparator function
		 * @param other Another native Guid object to compare against
		 * @return bool True if the Guids match, false if they do not
		*/
		inline bool Equals(const Guid& other) const { return CompareGuid(*this, other) == 0; }

		bool operator==(const Guid& other) const
		{
			return CompareGuid(*this, other) == 0;
		}

		bool operator!=(const Guid& other) const
		{
			return CompareGuid(*this, other) != 0;
		}

		bool operator<(const Guid& other) const
		{
			return CompareGuid(*this, other) == -1;
		}

		bool operator<=(const Guid& other) const
		{
			return CompareGuid(*this, other) != 1;
		}

		bool operator>(const Guid& other) const
		{
			return CompareGuid(*this, other) == 1;
		}

		bool operator>=(const Guid& other) const
		{
			return CompareGuid(*this, other) != -1;
		}

	private:
		Guid();

		std::string m_StringGuid;

		/**
		 * @brief Compares GUIDs using platform-specific comparison algorithms
		 * @param a First native GUID to compare with
		 * @param b Second native GUID to compare against
		 * @return Integer representing result
		 *		   -1 if a is less than b
		 *		    0 if a is equal to b
		 *		    1 if a is greater than b
		*/
		static int CompareGuid(const Guid& a, const Guid& b);

#ifdef _SAD_WINDOWS
		Guid(UUID uuid);
		UUID m_Guid;
#else
		Guid(uuid_t uuid);
		uuid_t m_Guid;
#endif
	};

}
