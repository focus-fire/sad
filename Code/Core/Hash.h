#pragma once

#include <bit>
#include <bitset>
#include <random>

namespace core
{
	/**
	 * @brief Set of helpers and utilities for instrumenting hashes. 
	 *		  The core::HashCombine uses multiple xor shifts and a binary rotation.
	 *
	 * @reference http://burtleburtle.net/bob/hash/doobs.html
	 * @reference https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key/57556517#57556517
	 * @reference https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values
	*/

	/**
	 * @brief Performs an XOR of n against n right-shifted to a degree i
	 * @tparam T Type being XOR'ed and right shifted, must have respective operators implemented
	 * @param n Variable to XOR 
	 * @param i Number of bits to shift right by
	*/
	template<typename T>
	inline T XorShift(const T& n, int i)
	{
		return n ^ (n >> i);
	}

	/**
	 * @brief Hash function pseudo-randomly distributing a 32-bit integer 
	*/
	inline uint32_t Distribute(const uint32_t& n)
	{
		uint32_t pattern = 0x55555555ul;			// Alternating 0s/1s
		uint32_t constant = 1067348297ul;		    // Random uneven integer
		return constant * XorShift(pattern * XorShift(n, 16), 16);
	}

	/**
	 * @brief Hash function pseudo-randomly distributing a 64-bit integer 
	*/
	inline uint64_t Distribute(const uint64_t& n)
	{
		uint64_t pattern = 0x5555555555555555ull;	 // Alternating 0s/1s
		uint64_t constant = 6238201529431549731ull;  // Random uneven integer
		return constant * XorShift(pattern * XorShift(n, 32), 32);
	}

	/**
	 * @brief Custom hash combine function, can be leveraged in a functor for hashing of STL 'unordered' data structures
	 * @tparam T Hashable type to rehash
	 * @param seed Old seed value to feed the hash 
	 * @param item New key being stored in the hash location
	 * @return The final hash
	 * @note Should be called with the old seed value and new key for best results. 
	 *       This gets hashed and combined to create the new seed value.
	*/
	template<class T>
	inline size_t HashCombine(std::size_t& seed, const T& item)
	{
		const uint32_t n = std::hash<T>{}(item);
		return std::rotl(static_cast<size_t>(seed), std::numeric_limits<size_t>::digits / 3) ^ Distribute(n);
	}
}
