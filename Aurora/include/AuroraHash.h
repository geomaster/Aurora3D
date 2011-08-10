#ifndef __AURORA_HASH_H__
#define __AURORA_HASH_H__

#include <string>
#include <cassert>
#include "AuroraPrereqs.h"
#include <boost/unordered_map.hpp>

namespace Aurora
{
#	define DEFINE_HASH(name, type) \
	template <> \
	class AURORA_LIBRARY name < type > \
	{ \
	public: \
		std::size_t operator ()(const type &key) const; \
	}

#	define HASH_FUNCTION(name, type) \
	std::size_t name < type > ::operator ()(const type &key) const

#	define SPECIALIZE_HASH(name, type) \
	std::size_t name < type > ::operator ()(const type &key) const \
	{ \
		return static_cast<std::size_t>(key); \
	}

	template <typename T>
	class ApproximateHash
	{
	public:
		std::size_t operator() (const T& key) const
		{
			return static_cast<std::size_t>(key);
		}
	};

	template <typename T>
	class Hash
	{
	public:
		std::size_t operator() (const T& key) const
		{
			return static_cast<std::size_t>(key);
		}

		std::size_t operator() (const T* const key) const
		{
			return reinterpret_cast<std::size_t>(key);
		}
	};

	template<>
	class Hash<void*>
	{
	public:
		std::size_t operator() (const void* const key) const
		{
			return reinterpret_cast<std::size_t>(key);
		}
	};

	DEFINE_HASH(ApproximateHash, String);

	DEFINE_HASH(Hash, String);
	DEFINE_HASH(Hash, int);
	DEFINE_HASH(Hash, unsigned int);
	DEFINE_HASH(Hash, long);
	DEFINE_HASH(Hash, unsigned long);
	DEFINE_HASH(Hash, short);
	DEFINE_HASH(Hash, unsigned short);
	DEFINE_HASH(Hash, char);
	DEFINE_HASH(Hash, unsigned char);
	DEFINE_HASH(Hash, float);
	DEFINE_HASH(Hash, double);
}

#endif // __AURORA_HASH_H__
