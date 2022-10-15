#pragma once
#include <vector>
#include <functional>
#include <queue>


class TraverseCallbacks {
	typedef std::function<void(size_t)> vertex_callback_type;
	typedef std::function<void(std::pair<size_t, size_t>, bool)> edge_callback_type;
public:
	vertex_callback_type enter_callback;
	vertex_callback_type exit_callback;
	vertex_callback_type visit_callback;
	edge_callback_type edge_callback;

	TraverseCallbacks() {
		enter_callback = [](size_t) {};
		exit_callback = [](size_t) {};
		visit_callback = [](size_t) {};
		edge_callback = [](std::pair<size_t, size_t>, bool) {};
	}
	TraverseCallbacks& set_enter_callback(vertex_callback_type callback) { enter_callback = callback; return *this; }
	TraverseCallbacks& set_exit_callback(vertex_callback_type callback) { exit_callback = callback; return *this; }
	TraverseCallbacks& set_visit_callback(vertex_callback_type callback) { visit_callback = callback; return *this; }
	TraverseCallbacks& set_edge_callback(edge_callback_type callback) { edge_callback = callback; return *this; }
};


void bfs(
	size_t start_vertex,
	const std::vector<std::vector<size_t>>& neighbours,
	TraverseCallbacks callbacks
) {
	std::vector<size_t> visited(neighbours.size(), false);

	std::queue<size_t> queue;
	queue.push(start_vertex);
	visited[start_vertex] = true;
	while (!queue.empty()) {
		auto vertex = queue.front();
		queue.pop();

		callbacks.visit_callback(vertex);

		for (const auto child : neighbours[vertex]) {
			callbacks.edge_callback(std::make_pair(vertex, child), !visited[child]);
			if (!visited[child]) {
				visited[child] = true;
				queue.push(child);
			}
		}
	}
}


void dfs(
	size_t start_vertex,
	const std::vector<std::vector<size_t>>& neighbours,
	TraverseCallbacks callbacks
) {
	std::vector<size_t> visited(neighbours.size(), false);
	std::vector<size_t> next_neighbour_to_visit(neighbours.size(), 0);
	std::vector<size_t> stack;
	stack.reserve(neighbours.size());

	stack.push_back(start_vertex);
	callbacks.enter_callback(start_vertex);

	auto has_unused_neighbours = [&](size_t vertex) {return next_neighbour_to_visit[vertex] < neighbours[vertex].size(); };
	auto get_neighbour = [&](size_t vertex) {return neighbours[vertex][next_neighbour_to_visit[vertex]]; };

	while (!stack.empty()) {
		size_t vertex = stack.back();
		callbacks.visit_callback(vertex);
		visited[vertex] = true;

		// skipping already visited neighbours
		while (has_unused_neighbours(vertex) && visited[get_neighbour(vertex)]) {
			auto neighbour = get_neighbour(vertex);
			callbacks.edge_callback(std::make_pair(vertex, neighbour), false);
			next_neighbour_to_visit[vertex]++;
		}

		if (!has_unused_neighbours(vertex)) { // all children dfs-ed, exiting subtree
			callbacks.exit_callback(vertex);
			stack.pop_back();
		}
		else { // entering next child
			auto neighbour = get_neighbour(vertex);
			stack.push_back(neighbour);
			callbacks.edge_callback(std::make_pair(vertex, neighbour), true);
			callbacks.enter_callback(neighbour);
			next_neighbour_to_visit[vertex]++;
		}
	}
}
