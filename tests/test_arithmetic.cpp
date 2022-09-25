#include "../src/arithmetic.h"

#define CATCH_CONFIG_MAIN
#include "../Catch2/extras/catch_amalgamated.hpp"

#include <type_traits>


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


TEST_CASE("fast_pow") {
    SECTION("with ints") {
        CHECK(fast_pow(2, 0) == 1);
        CHECK(fast_pow(2, 1) == 2);
        CHECK(fast_pow(2, 2) == 4);
        CHECK(fast_pow(2ll, 60) == 1152921504606846976);
    }
    SECTION("it does not take forever to calculate large powers") {
        auto start = std::chrono::system_clock::now();
        CHECK(fast_pow(1, 1000) == 1);
        auto end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1);

        start = std::chrono::system_clock::now();
        CHECK(fast_pow(1, 100000) == 1);
        end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1);

        start = std::chrono::system_clock::now();
        CHECK(fast_pow(1, 10000000) == 1);
        end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1);

        start = std::chrono::system_clock::now();
        CHECK(fast_pow(1, 1000000000) == 1);
        end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1);
    }
    SECTION("works with computations by modulo") {
        CHECK(fast_pow(Modular<17>(2), 0).value == 1);
        CHECK(fast_pow(Modular<17>(2), 2).value == 4);
        CHECK(fast_pow(Modular<17>(2), 4).value == 16);
        CHECK(fast_pow(Modular<17>(2), 5).value == 15);
        CHECK(fast_pow(Modular<17>(2), 17).value == 2);
        CHECK(fast_pow(Modular<998244353>(2), 998244353 - 1).value == 1);
    }
}


TEST_CASE("Modular") {
    SECTION("constructor") {
        CHECK(Modular<7>(0).value == 0);
        CHECK(Modular<7>(2).value == 2);
        CHECK(Modular<7>(77777).value == 0);
        CHECK(Modular<7>(77778).value == 1);
    }
    SECTION("operator+=") {
        Modular<7> value(2);
        CHECK(value.value == 2);
        value += Modular<7>(0);
        CHECK(value.value == 2);
        value += Modular<7>(2);
        CHECK(value.value == 4);
        value += 704;
        CHECK(value.value == 1);
    }
    SECTION("operator-=") {
        Modular<7> value(2);
        CHECK(value.value == 2);
        value -= Modular<7>(0);
        CHECK(value.value == 2);
        value -= Modular<7>(2);
        CHECK(value.value == 0);
        value -= 704;
        CHECK(value.value == 3);
    }
    SECTION("operator*=") {
        Modular<7> value(2);
        CHECK(value.value == 2);
        value *= Modular<7>(2);
        CHECK(value.value == 4);
        value *= 704;
        CHECK(value.value == 2);
    }
    SECTION("operator+") {
        CHECK((Modular<7>(2) + Modular<7>(0)).value == 2);
        CHECK((Modular<7>(2) + Modular<7>(3)).value == 5);
        CHECK((Modular<7>(2) + 704).value == 6);
    }
    SECTION("operator-") {
        CHECK((Modular<7>(2) - Modular<7>(0)).value == 2);
        CHECK((Modular<7>(2) - Modular<7>(3)).value == 6);
        CHECK((Modular<7>(2) - 704).value == 5);
    }
    SECTION("operator*") {
        CHECK((Modular<7>(2) * Modular<7>(0)).value == 0);
        CHECK((Modular<7>(2) * Modular<7>(1)).value == 2);
        CHECK((Modular<7>(2) * Modular<7>(3)).value == 6);
        CHECK((Modular<7>(2) * 704).value == 1);
    }
    SECTION("operator/=") {
        Modular<7> value(2);
        CHECK(value.value == 2);
        value /= Modular<7>(3);
        CHECK(value.value == 3);
        value *= Modular<7>(3);
        CHECK(value.value == 2);
    }
    SECTION("operator/") {
        CHECK_THROWS((Modular<7>(2) / Modular<7>(0)));
        CHECK((Modular<7>(2) / Modular<7>(1)).value == 2);
        CHECK((Modular<7>(2) / Modular<7>(3)).value == 3);
        CHECK((Modular<7>(2) / 6).value == 5);
        CHECK(((Modular<1000000007>(1) / Modular<1000000007>(22222)) * 22222).value == 1);
    }
    SECTION("inverse") {
        SECTION("0 has no opposite") {
            CHECK_THROWS(Modular<5>(0).inverse());
        }
        SECTION("opposite of 1 is 1") {
            CHECK(Modular<5>(1).inverse().value == 1);
        }
        SECTION("correctly finds opposite for every positive number with prime MOD") {
            const int64_t MOD = 17;
            for (int64_t i = 1; i < MOD; ++i) {
                auto opposite = Modular<MOD>(i).inverse();
                CHECK((opposite * i).value == 1);
            }
        }
        SECTION("with non prime MOD number either has the oposite or has gcd with MOD > 1") {
            const int64_t MOD = 18;
            for (int64_t i = 1; i < MOD; ++i) {
                auto g = gcd(i, MOD);
                if (g == 1) {
                    auto opposite = Modular<MOD>(i).inverse();
                    CHECK((opposite * i).value == 1);
                }
                else {
                    CHECK_THROWS(Modular<MOD>(i).inverse());
                }
            }
        }
    }
}

TEST_CASE("ModularFactorials") {
    SECTION("is singleton") {
        // checking that first computing of 1000000! takes time
        auto start = std::chrono::system_clock::now();
        ModularFactorials<1000000007>::instance().factorial(1000000);
        auto end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > 50);

        // checking that second computing of 1000000! does not take a lot of time
        start = std::chrono::system_clock::now();
        ModularFactorials<1000000007>::instance().factorial(1000000);
        end = std::chrono::system_clock::now();
        REQUIRE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1);
    }
    SECTION("is not constructible") {
        CHECK(std::is_constructible<ModularFactorials<1000000007>>::value == false);
    }
    SECTION("first few factorials") {
        CHECK(ModularFactorials<1000000007>::instance().factorial(0).value == 1);
        CHECK(ModularFactorials<1000000007>::instance().factorial(1).value == 1);
        CHECK(ModularFactorials<1000000007>::instance().factorial(2).value == 2);
        CHECK(ModularFactorials<1000000007>::instance().factorial(3).value == 6);
        CHECK(ModularFactorials<1000000007>::instance().factorial(4).value == 24);
        CHECK(ModularFactorials<1000000007>::instance().factorial(5).value == 120);
        CHECK(ModularFactorials<1000000007>::instance().factorial(6).value == 720);
    }
    SECTION("combinations") {
        CHECK(ModularFactorials<1000000007>::instance().combinations(4, 0).value == 1);
        CHECK(ModularFactorials<1000000007>::instance().combinations(4, 1).value == 4);
        CHECK(ModularFactorials<1000000007>::instance().combinations(4, 2).value == 6);
        CHECK(ModularFactorials<1000000007>::instance().combinations(4, 3).value == 4);
        CHECK(ModularFactorials<1000000007>::instance().combinations(4, 4).value == 1);
        CHECK(ModularFactorials<1000000007>::instance().combinations(50, 6).value == (50ll * 49ll * 48ll * 47ll * 46ll * 45ll) / (1 * 2 * 3 * 4 * 5 * 6));
    }
}
