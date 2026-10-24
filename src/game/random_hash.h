#ifndef GAME_RANDOM_HASH_H
#define GAME_RANDOM_HASH_H

#include <initializer_list>

// Stateless random numbers for game physics that the client has to predict.
// The result only depends on the arguments, which the server and the client
// both know, so unlike `CPrng` it doesn't matter in which order or how often
// they are generated.
namespace RandomHash
{
	// Each kind of random decision uses its own source, so that two
	// different decisions with the same values aren't correlated.
	enum class ESource
	{
		CHARACTER_TELEPORT,
		HOOK_TELEPORT,
		PROJECTILE_TELEPORT,
		UNOWNED_PROJECTILE_TELEPORT,
		LASER_TELEPORT,
		UNOWNED_LASER_TELEPORT,
	};

	// Returns a number in the range [0, Below) derived from the seed, the
	// source and the values. Within one source, the values must uniquely
	// identify the decision, e.g. by an entity id and the game tick.
	// Otherwise two decisions would always have the same result.
	int SeededRandomIntBelow(int Below, int Seed, ESource Source, std::initializer_list<int> Values);
} // namespace RandomHash

#endif // GAME_RANDOM_HASH_H
