#include "header.h"

int64_t gcdExtended(int64_t a, int64_t b, int64_t& x, int64_t& y)
// ax + by= gcd(a,b).
{
	// Base Case
	if (a == 0)
	{
		x = 0;
		y = 1;
		return b;
	}

	int64_t x1, y1; // To store results of recursive call
	int64_t gcd = gcdExtended(b % a, a, x1, y1);

	// Update x and y using results of
	// recursive call
	x = y1 - (b / a) * x1;
	y = x1;

	return gcd;
}

