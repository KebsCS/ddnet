#include "test.h"

#include <game/random_hash.h>

#include <gtest/gtest.h>

using RandomHash::ESource;
using RandomHash::SeededRandomIntBelow;

TEST(RandomHash, SingleResult)
{
	EXPECT_EQ(SeededRandomIntBelow(0, 1234, ESource::CHARACTER_TELEPORT, {1, 2, 3}), 0);
	EXPECT_EQ(SeededRandomIntBelow(1, 1234, ESource::CHARACTER_TELEPORT, {1, 2, 3}), 0);
}

TEST(RandomHash, Range)
{
	for(int Below : {2, 3, 7, 100})
	{
		for(int Tick = 0; Tick < 1000; Tick++)
		{
			const int Result = SeededRandomIntBelow(Below, 1234, ESource::CHARACTER_TELEPORT, {5, Tick, 42});
			EXPECT_GE(Result, 0);
			EXPECT_LT(Result, Below);
		}
	}
}

TEST(RandomHash, Deterministic)
{
	for(int Tick = 0; Tick < 1000; Tick++)
	{
		EXPECT_EQ(
			SeededRandomIntBelow(1 << 30, 1234, ESource::PROJECTILE_TELEPORT, {5, 100, Tick, 42}),
			SeededRandomIntBelow(1 << 30, 1234, ESource::PROJECTILE_TELEPORT, {5, 100, Tick, 42}));
	}
}

TEST(RandomHash, Uniform)
{
	int aCounts[3] = {0, 0, 0};
	for(int Tick = 0; Tick < 30000; Tick++)
		aCounts[SeededRandomIntBelow(3, 1234, ESource::CHARACTER_TELEPORT, {5, Tick, 42})]++;
	for(int Count : aCounts)
	{
		EXPECT_GT(Count, 9500);
		EXPECT_LT(Count, 10500);
	}
}

TEST(RandomHash, IndependentInputs)
{
	constexpr int BELOW = 1 << 30;
	int SameSource = 0;
	int SameSeed = 0;
	int SameValue = 0;
	for(int Tick = 0; Tick < 1000; Tick++)
	{
		const int Result = SeededRandomIntBelow(BELOW, 1234, ESource::CHARACTER_TELEPORT, {5, Tick});
		if(Result == SeededRandomIntBelow(BELOW, 1234, ESource::HOOK_TELEPORT, {5, Tick}))
			SameSource++;
		if(Result == SeededRandomIntBelow(BELOW, 1235, ESource::CHARACTER_TELEPORT, {5, Tick}))
			SameSeed++;
		if(Result == SeededRandomIntBelow(BELOW, 1234, ESource::CHARACTER_TELEPORT, {6, Tick}))
			SameValue++;
	}
	EXPECT_LE(SameSource, 1);
	EXPECT_LE(SameSeed, 1);
	EXPECT_LE(SameValue, 1);
}
