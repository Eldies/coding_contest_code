#pragma once
#include <cstdint>
#include <stdexcept>


int64_t gcdExtended(int64_t a, int64_t b, int64_t& x, int64_t& y)
// ax + by = gcd(a,b).
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


int64_t gcd(int64_t a, int64_t b) {
	int64_t x, y;
	return gcdExtended(a, b, x, y);
}


int64_t modInverse(int64_t a, int64_t MOD)
{
	int64_t x, y;
	int64_t g = gcdExtended(a, MOD, x, y);
	if (g != 1)
		throw std::invalid_argument("Number does not have opposite with this MOD");
	return (x % MOD + MOD) % MOD;
}


template <typename T>
T fast_pow(T n, int power) {
	T result(1);

	while (power > 0) {
		if (power % 2 == 1) {
			result = (result * n);
		}
		n = n * n;
		power >>= 1;
	}

	return result;
}


template <uint64_t MOD> // should work for every MOD such that MOD^2 < 2^64
struct Modular {
	uint64_t value;

	Modular(uint64_t value) : value(value % MOD) {}
	Modular& operator=(const Modular& right) { value = right.value; return *this; }

	Modular& operator+=(const Modular& right) { value = (value + right.value) % MOD; return *this; }
	Modular& operator*=(const Modular& right) { value = (value * right.value) % MOD; return *this; }
	Modular& operator-=(const Modular& right) { value = (MOD + value - right.value) % MOD; return *this; }
	Modular operator+(const Modular& right) const { Modular result(value); result += right; return result; }
	Modular operator*(const Modular& right) const { Modular result(value); result *= right; return result; }
	Modular operator-(const Modular& right) const { Modular result(value); result -= right; return result; }

	Modular& operator/=(const Modular& right) {
		value = (value * modInverse(right.value, MOD)) % MOD;
		return *this; 
	}
	Modular operator/(const Modular& right) const { Modular result(value); result /= right; return result; }
};
