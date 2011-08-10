#include "AuroraHash.h"

using namespace Aurora;

// Removed ASM code because of the portability and wchar_t issues. See AuroraHash.cpp.bak.

HASH_FUNCTION(ApproximateHash, String)
{
	std::size_t hash = 2166136261U;
	std::size_t len = key.length();
	std::size_t step = len / 10 + 1;

	for (unsigned int i = 0; i < len; i += step)
	{
		hash = hash * 16777619U ^ static_cast<std::size_t>(key[i]);
	}

	return hash;
}

HASH_FUNCTION(Hash, String)
{
	std::size_t hash = 2166136261U;

	for (String::const_iterator it = key.begin(); it != key.end(); ++it)
		hash = hash * 16777619U ^ *it;

	return hash;
}


SPECIALIZE_HASH(Hash, int);
SPECIALIZE_HASH(Hash, unsigned int);
SPECIALIZE_HASH(Hash, long);
SPECIALIZE_HASH(Hash, unsigned long);
SPECIALIZE_HASH(Hash, short);
SPECIALIZE_HASH(Hash, unsigned short);
SPECIALIZE_HASH(Hash, char);
SPECIALIZE_HASH(Hash, unsigned char);

// Here we recognize two special cases: with float and double
// datatypes.
// We can cast these to unsigned integers (bitwise, no expensive
// conversion), and this will provide a fairly good distribution.
// Only if we have a negative number, we negate the number, 
// as in two's complement. Furthermore, with double precision,
// we need to xor-fold the lower 32 bits to the higher.

HASH_FUNCTION(Hash, float)
{
	assert(sizeof(float) <= sizeof(std::size_t));
	std::size_t keyCast = *reinterpret_cast<const std::size_t*>(&key);
	std::size_t sign = keyCast & 0xF0000000;
	if (sign) keyCast = sign | ~(keyCast & 0x0FFFFFFF);

	return keyCast;
}

HASH_FUNCTION(Hash, double)
{
	unsigned char doubleHalf = sizeof(double) / 2;
	assert(doubleHalf <= sizeof(std::size_t));

	const std::size_t* pKeyCast = reinterpret_cast<const std::size_t*>(&key);

	// While it may seem so, this doesn't depend on endianness: both are xor'd
	// at the end, so it doesn't matter if the high and low bits are inverted
	std::size_t keyHigh = *pKeyCast;
	std::size_t keyLow = *(pKeyCast + doubleHalf);
	std::size_t sign = (keyHigh | keyLow) & 0xF0000000;

	return (sign? sign | ~((keyHigh ^ keyLow) & 0x0FFFFFFF) : keyHigh ^ keyLow);
}