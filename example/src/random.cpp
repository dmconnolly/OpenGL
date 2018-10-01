#include "random.hpp"

/// Empty constructor
Random::Random() {
	/* Empty */
}

/// Empty destructor
Random::~Random() {
	/* Empty */
}

/// Return random int in range
int Random::range(int min, int max) {
	return rng.uniform(min, max);
}

/// Return random float in range
float Random::range(float min, float max) {
	return rng.uniform(min, max);
}
