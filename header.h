#pragma once
#include <cstdint>
#include <stdexcept>


int64_t gcdExtended(int64_t, int64_t, int64_t&, int64_t&);
int64_t gcd(int64_t a, int64_t b);
int64_t modInverse(int64_t a, int64_t MOD);
