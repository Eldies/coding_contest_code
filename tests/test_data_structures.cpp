#include "../src/data_structures.h"

#define CATCH_CONFIG_MAIN
#include "../Catch2/extras/catch_amalgamated.hpp"


SCENARIO("DisjointSetUnion") {
    GIVEN("freshly constructed DisjointSetUnion") {
        DisjointSetUnion dsu(10);
        
        THEN("all sets are disjointed and have size = 1") {
            for (int i = 0; i < 10; ++i) {
                CHECK(dsu.find_set(i) == i);
                CHECK(dsu.get_set_size(i) == 1);
            }
        }

        WHEN("joining not joined sets") {
            THEN("join_sets returns true") {
                CHECK(dsu.join_sets(0, 1) == true);
                CHECK(dsu.join_sets(0, 2) == true);
                CHECK(dsu.join_sets(0, 3) == true);
                CHECK(dsu.join_sets(4, 3) == true);
            }
        }

        WHEN("sets 0-3 are joined") {
            dsu.join_sets(0, 1);
            dsu.join_sets(0, 2);
            dsu.join_sets(0, 3);

            THEN("all sets 0-3 have the same set number and the same size=4") {
                CHECK(dsu.find_set(0) == dsu.find_set(1));
                CHECK(dsu.find_set(0) == dsu.find_set(2));
                CHECK(dsu.find_set(0) == dsu.find_set(3));
                CHECK(dsu.get_set_size(0) == 4);
                CHECK(dsu.get_set_size(1) == 4);
                CHECK(dsu.get_set_size(2) == 4);
                CHECK(dsu.get_set_size(3) == 4);
            }

            AND_WHEN("joining already joined sets") {
                THEN("join_sets returns false") {
                    for (int i = 0; i < 4; ++i) {
                        for (int j = i; j < 4; ++j) {
                            CHECK(dsu.join_sets(i, j) == false);
                        }
                    }
                }
            }

            AND_WHEN("sets 4-6 are joined") {
                dsu.join_sets(4, 5);
                dsu.join_sets(4, 6);

                THEN("sets 0 and 4 are not joined") {
                    CHECK(dsu.find_set(0) != dsu.find_set(4));
                }
                THEN("all sets 4-6 have the same set number and the same size=3") {
                    CHECK(dsu.find_set(4) == dsu.find_set(5));
                    CHECK(dsu.find_set(4) == dsu.find_set(6));
                    CHECK(dsu.get_set_size(4) == 3);
                    CHECK(dsu.get_set_size(5) == 3);
                    CHECK(dsu.get_set_size(6) == 3);
                }

                AND_WHEN("joined 0 and 4") {
                    REQUIRE(dsu.join_sets(0, 4) == true);

                    THEN("all sets 0-6 have the same set number and the same size=7") {
                        for (int i = 0; i < 7; ++i) {
                            CHECK(dsu.find_set(i) == dsu.find_set(0));
                            CHECK(dsu.get_set_size(i) == 7);
                        }
                    }
                }
            }
        }
    }
}


TEST_CASE("DisjointSetUnion speed") {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto measure_time_for_size = [&gen](int size) {
        std::uniform_int_distribution<> distrib(0, size - 1);

        auto start = std::chrono::system_clock::now();
        for (int n = 0; n < 100; ++n) {
            DisjointSetUnion dsu(size);
            for (int i = 0; i < size; ++i) {
                dsu.join_sets(distrib(gen), distrib(gen));
            }
            for (int i = 0; i < size; ++i) {
                dsu.find_set(i);
            }

        }
        auto end = std::chrono::system_clock::now();

        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    };

    auto time100 = measure_time_for_size(100);
    auto time10000 = measure_time_for_size(10000);

    CHECK(time10000 == Catch::Approx(time100 * 100).epsilon(0.2));
}
