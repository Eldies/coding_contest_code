#include "../src/graphs.h"

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER
#include "../Catch2/extras/catch_amalgamated.hpp"


SCENARIO("Graph trversal") {
    GIVEN("graph as list of neighbours for every vertex") {
        std::vector<std::vector<size_t>> neighbours = {
            {1},
            {2, 4},
            {1, 6},
            {1},
            {5},
            {1, 6},
            {5}
        };

        typedef std::vector<std::pair<std::pair<size_t, size_t>, bool>> edges_vector_type;

        AND_GIVEN("starting vertex is 0") {
            WHEN("Doing Breadth-first search") {
                edges_vector_type expected_edges = {
                    { { 0, 1 }, true },
                    { { 1, 2 }, true },
                    { { 1, 4 }, true },
                    { { 2, 1 }, false },
                    { { 2, 6 }, true },
                    { { 4, 5 }, true },
                    { { 6, 5 }, false },
                    { { 5, 1 }, false },
                    { { 5, 6 }, false }
                };

                edges_vector_type visited_edges;
                bfs(
                    0,
                    neighbours,
                    [&visited_edges](std::pair<size_t, size_t> edge, bool b) { visited_edges.push_back(std::make_pair(edge, b)); }
                );

                THEN("edges visited in correct order") {
                    CHECK(visited_edges == expected_edges);
                }
            }
        }
    }
}
