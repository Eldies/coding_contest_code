#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>


int64_t gcdExtended(int64_t a, int64_t b, int64_t& x, int64_t& y) 
// ax + by = gcd(a,b).
// O(log(min(a,b)))
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
	// O(log(min(a,b)))
	int64_t x, y;
	return gcdExtended(a, b, x, y);
}


template <typename T>
T fast_pow(T n, int power) {
	// O(log(power))
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
	Modular& operator=(const Modular& right) { value = right.value; return *this; } // O(1)

	Modular& operator+=(const Modular& right) { value = (value + right.value) % MOD; return *this; } // O(1)
	Modular& operator*=(const Modular& right) { value = (value * right.value) % MOD; return *this; } // O(1)
	Modular& operator-=(const Modular& right) { value = (MOD + value - right.value) % MOD; return *this; } // O(1)
	Modular operator+(const Modular& right) const { Modular result(value); result += right; return result; } // O(1)
	Modular operator*(const Modular& right) const { Modular result(value); result *= right; return result; } // O(1)
	Modular operator-(const Modular& right) const { Modular result(value); result -= right; return result; } // O(1)

	Modular& operator/=(const Modular& right) { return (*this *= right.inverse()); } // O(log(right.value))
	Modular operator/(const Modular& right) const { Modular result(value); result /= right; return result; } // O(log(right.value))

	Modular inverse() const
	{
		// O(log(value))
		int64_t x, y;
		int64_t g = gcdExtended(value, MOD, x, y);
		if (g != 1)
			throw std::invalid_argument("Number " + std::to_string(value) + " does not have opposite with MOD " + std::to_string(MOD));
		if (x < 0) {
			x += ((std::abs(x) / MOD) + 1) * MOD;
		}
		return Modular(x);
	}
};


template <uint64_t MOD>
class ModularFactorials {
	std::vector<Modular<MOD>> factorials;
	ModularFactorials() : factorials(1, 1) {}
public:
	static ModularFactorials& instance() {
		static ModularFactorials mf;
		return mf;
	}
	ModularFactorials(const ModularFactorials&) = delete;
	Modular<MOD> factorial(int value) {
		// O(1) per request + O(max(value)) once
		if (value >= factorials.size()) {
			for (int i = factorials.size(); i <= value; ++i) {
				factorials.push_back(factorials.back() * i);
			}
		}
		return factorials[value];
	}
	Modular<MOD> combinations(int n, int k) {
		// O(log(MOD)) per request + O(max(n)) once
		return factorial(n) / (factorial(k) * factorial(n - k));
	}
};
