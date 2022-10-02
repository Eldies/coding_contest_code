#include "../src/numerical.h"

#define CATCH_CONFIG_MAIN
#include "../Catch2/extras/catch_amalgamated.hpp"


TEST_CASE("minimum_ternary_search") {
    double result;
    int count;
    SECTION("borders coincide") {
        CHECK(minimum_ternary_search(0., 0., [](double) { return 1.; }) == 0);
    }
    SECTION("parabola") {
        result = minimum_ternary_search(-1., 1., [](double x) { return x * x; });
        CHECK(std::abs(result) < 1e-7);
        result = minimum_ternary_search(-10000., 1000000., [](double x) { return x * x; });
        CHECK(std::abs(result) < 1e-7);
        result = minimum_ternary_search(-10000., 1000000., [](double x) { x -= 123.456789; return x * x; });
        CHECK(std::abs(result - 123.456789) / 123.456789 < 1e-7);
    }
    SECTION("number of operations") {
        double golden_ratio = (std::sqrt(5) + 1) / 2;
        count = 0;
        minimum_ternary_search(-1., 1., [&count](double x) { count++; return x * x; });
        CHECK(count == 2 + std::ceil(std::log(2. / 1e-7) / std::log(golden_ratio)));
        count = 0;
        minimum_ternary_search(-100., 100., [&count](double x) { count++; return x * x; });
        CHECK(count == 2 + std::ceil(std::log(200. / 1e-7) / std::log(golden_ratio)));
        count = 0;
        minimum_ternary_search(-1000000., 100000000., [&count](double x) { count++; return x * x; });
        CHECK(count == 2 + std::ceil(std::log(101000000. / 1e-7) / std::log(golden_ratio)));
        count = 0;
        minimum_ternary_search(-1000000., 100000000., [&count](double x) { count++; return x * x; }, 1e-3);
        CHECK(count == 2 + std::ceil(std::log(101000000. / 1e-3) / std::log(golden_ratio)));
    }
}
