#pragma once
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "pcg/pcg_utils.hpp"

/// Wrapper for pcg random number generator
class Random {
private:
	/// Random number generator
	pcg_utils::mt19937_rng rng;
public:
	Random();
	~Random();
	int range(int min, int max);
	float range(float min, float max);
};

#endif /* RANDOM_HPP */
