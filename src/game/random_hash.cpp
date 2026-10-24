#include "random_hash.h"

#include <cstdint>

// MurmurHash3 32 bit finalizer. It has known good bias and avalanche
// properties. Marginally better 32 bit mixers exist, but this one is well
// tested and good enough.
static uint32_t Murmur3Mix32(uint32_t Value)
{
	Value ^= Value >> 16;
	Value *= 0x85ebca6bu;
	Value ^= Value >> 13;
	Value *= 0xc2b2ae35u;
	Value ^= Value >> 16;
	return Value;
}

static void HashAppend(uint32_t &State, int Value)
{
	State = Murmur3Mix32(State ^ static_cast<uint32_t>(Value));
}

namespace RandomHash
{
	int SeededRandomIntBelow(int Below, int Seed, ESource Source, std::initializer_list<int> Values)
	{
		if(Below <= 1)
			return 0;

		uint32_t State = 0x1234567u;
		HashAppend(State, Seed);
		HashAppend(State, static_cast<int>(Source));
		for(int Value : Values)
			HashAppend(State, Value);

		// Slightly biased if `Below` is not a power of two, like `CWorldCore::RandomOr0`
		return static_cast<int>(Murmur3Mix32(State) % static_cast<uint32_t>(Below));
	}
} // namespace RandomHash
