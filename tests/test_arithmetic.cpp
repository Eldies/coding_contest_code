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

