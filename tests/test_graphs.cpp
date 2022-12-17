#include "../src/graphs.h"

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER
#include "../Catch2/extras/catch_amalgamated.hpp"


SCENARIO("Graph traversal") {
    GIVEN("graph as list of neighbours for every vertex") {
        std::vector<std::vector<size_t>> neighbours = {
            {1},
            {2, 4},
            {1, 6, 4},
            {1},
            {5},
            {1, 6},
            {5}
        };

        typedef std::vector<std::pair<std::pair<size_t, size_t>, bool>> edges_vector_type;

        WHEN("Doing Breadth-first search from vertex 0") {
            std::vector<size_t> expected_vertexes = {0, 1, 2, 4, 6, 5};
            edges_vector_type expected_edges = {
                { { 0, 1 }, true },
                { { 1, 2 }, true },
                { { 1, 4 }, true },
                { { 2, 1 }, false },
                { { 2, 6 }, true },
                { { 2, 4 }, false },
                { { 4, 5 }, true },
                { { 6, 5 }, false },
                { { 5, 1 }, false },
                { { 5, 6 }, false }
            };

            std::vector<size_t> visited_vertexes;
            edges_vector_type visited_edges;
            bfs(
                0,
                neighbours,
                TraverseCallbacks()
                .set_visit_callback([&visited_vertexes](size_t v) {visited_vertexes.push_back(v); })
                .set_edge_callback([&visited_edges](std::pair<size_t, size_t> edge, bool b) { visited_edges.push_back(std::make_pair(edge, b)); })
            );

            THEN("vertexes visited in correct order") {
                CHECK(visited_vertexes == expected_vertexes);
            }
            THEN("edges visited in correct order") {
                CHECK(visited_edges == expected_edges);
            }
        }

        WHEN("Doing Depth-first search from vertex 0") {
            edges_vector_type expected_edges = {
                { { 0, 1 }, true },
                { { 1, 2 }, true },
                { { 2, 1 }, false },
                { { 2, 6 }, true },
                { { 6, 5 }, true },
                { { 5, 1 }, false },
                { { 5, 6 }, false },
                { { 2, 4 }, true },
                { { 4, 5 }, false },
                { { 1, 4 }, false }
            };

            std::vector<size_t> entered_vertexes;
            std::vector<size_t> exited_vertexes;
            std::vector<size_t> visited_vertexes;
            edges_vector_type visited_edges;
            dfs(
                0,
                neighbours,
                TraverseCallbacks()
                .set_enter_callback([&entered_vertexes](size_t v) {entered_vertexes.push_back(v); })
                .set_exit_callback([&exited_vertexes](size_t v) {exited_vertexes.push_back(v); })
                .set_visit_callback([&visited_vertexes](size_t v) {visited_vertexes.push_back(v); })
                .set_edge_callback([&visited_edges](std::pair<size_t, size_t> edge, bool b) { visited_edges.push_back(std::make_pair(edge, b)); })
            );

            THEN("vertexes entered in correct order") {
                CHECK(entered_vertexes == std::vector<size_t>{0, 1, 2, 6, 5, 4});
            }
            THEN("vertexes exited in correct order") {
                CHECK(exited_vertexes == std::vector<size_t>{5, 6, 4, 2, 1, 0});
            }
            THEN("vertexes visited in correct order") {
                CHECK(visited_vertexes == std::vector<size_t>{0, 1, 2, 6, 5, 6, 2, 4, 2, 1, 0});
            }
            THEN("edges visited in correct order") {
                CHECK(visited_edges == expected_edges);
            }
        }

    }
}


SCENARIO("creating tree from graph") {
    GIVEN("graph as list of neighbours for every vertex") {
        std::vector<std::vector<size_t>> neighbours = {
            {1},
            {2, 4},
            {1, 6, 4},
            {1},
            {5},
            {1, 6},
            {5}
        };
    }
}


TEST_CASE("generate_tree_dfs") {
    SECTION("creating tree from graph") {
        std::vector<std::vector<size_t>> neighbours = {
            {1, 3},
            {2, 4},
            {1, 6, 4},
            {1},
            {5},
            {1, 6},
            {5}
        };
        Tree tree = generate_tree_dfs(neighbours);
        REQUIRE(tree.children[0] == std::vector<size_t>({ 1, 3 }));
        REQUIRE(tree.children[1] == std::vector<size_t>({ 2 }));
        REQUIRE(tree.children[2] == std::vector<size_t>({ 6, 4 }));
        REQUIRE(tree.children[3] == std::vector<size_t>({ }));
        REQUIRE(tree.children[4] == std::vector<size_t>({ }));
        REQUIRE(tree.children[5] == std::vector<size_t>({ }));
        REQUIRE(tree.children[6] == std::vector<size_t>({ 5 }));
        REQUIRE(tree.depth == std::vector<int>({ 0, 1, 2, 1, 3, 4, 3 }));
        REQUIRE(tree.parents == std::vector<int>({ -1, 0, 1, 0, 2, 6, 2 }));
    }
}
