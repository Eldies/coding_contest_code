#include "../header.h"

#define CATCH_CONFIG_MAIN
#include "../Catch2/extras/catch_amalgamated.hpp"


TEST_CASE("gcdExtended") {
    int64_t x, y;

    SECTION("basic") {
        CHECK(gcdExtended(22, 33, x, y) == 11);
        CHECK(x == -1);
        CHECK(y == 1);
    }
    SECTION("second is zero") {
        CHECK(gcdExtended(22, 0, x, y) == 22);
        CHECK(x == 1);
        CHECK(y == 0);
    }
    SECTION("first is zero") {
        CHECK(gcdExtended(0, 33, x, y) == 33);
        CHECK(x == 0);
        CHECK(y == 1);
    }
}

TEST_CASE("gcd") {
    SECTION("basic") {
        CHECK(gcd(22, 33) == 11);
    }
    SECTION("second is zero") {
        CHECK(gcd(22, 0) == 22);
    }
    SECTION("first is zero") {
        CHECK(gcd(0, 33) == 33);
    }
}

TEST_CASE("modInverse") {
    SECTION("opposite of 1 is 1") {
        CHECK(modInverse(1, 5) == 1);
    }
    SECTION("correctly finds opposite for every positive number with prime MOD") {
        int64_t MOD = 17;
        for (int64_t i = 1; i < MOD; ++i) {
            auto opposite = modInverse(i, MOD);
            CHECK((opposite * i) % MOD == 1);
        }
    }
    SECTION("with non prime MOD number either has the oposite or has gcd with MOD > 1") {
        int64_t MOD = 18;
        for (int64_t i = 1; i < MOD; ++i) {
            auto g = gcd(i, MOD);
            if (g == 1) {
                auto opposite = modInverse(i, MOD);
                CHECK((opposite * i) % MOD == 1);
            }
            else {
                CHECK_THROWS(modInverse(i, MOD));
            }
        }
    }
}
