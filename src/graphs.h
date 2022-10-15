#pragma once
#include <vector>
#include <functional>
#include <queue>


void bfs(
	size_t start_vertex,
	const std::vector<std::vector<size_t>>& neighbours,
	std::function<void(size_t)> callback_on_visit, 
	std::function<void(std::pair<size_t, size_t>, bool)> callback_on_edge // <from, to>, is_forward_edge
) {
	std::vector<size_t> visited(neighbours.size(), false);

	std::queue<size_t> queue;
	queue.push(start_vertex);
	visited[start_vertex] = true;
	while (!queue.empty()) {
		auto vertex = queue.front();
		queue.pop();

		callback_on_visit(vertex);

		for (const auto child : neighbours[vertex]) {
			callback_on_edge(std::make_pair(vertex, child), !visited[child]);
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
	std::function<void(size_t)> callback_on_enter,
	std::function<void(size_t)> callback_on_exit,
	std::function<void(size_t)> callback_on_visit,
	std::function<void(std::pair<size_t, size_t>, bool)> callback_on_edges // <from, to>, is_forward
) {
	std::vector<size_t> visited(neighbours.size(), false);
	std::vector<size_t> next_neighbour_to_visit(neighbours.size(), 0);
	std::vector<size_t> stack;
	stack.reserve(neighbours.size());

	stack.push_back(start_vertex);
	callback_on_enter(start_vertex);

	auto has_unused_neighbours = [&](size_t vertex) {return next_neighbour_to_visit[vertex] < neighbours[vertex].size(); };
	auto get_neighbour = [&](size_t vertex) {return neighbours[vertex][next_neighbour_to_visit[vertex]]; };

	while (!stack.empty()) {
		size_t vertex = stack.back();
		callback_on_visit(vertex);
		visited[vertex] = true;

		// skipping already visited neighbours
		while (has_unused_neighbours(vertex) && visited[get_neighbour(vertex)]) {
			auto neighbour = get_neighbour(vertex);
			callback_on_edges(std::make_pair(vertex, neighbour), false);
			next_neighbour_to_visit[vertex]++;
		}

		if (!has_unused_neighbours(vertex)) { // all children dfs-ed, exiting subtree
			callback_on_exit(vertex);
			stack.pop_back();
		}
		else { // entering next child
			auto neighbour = get_neighbour(vertex);
			stack.push_back(neighbour);
			callback_on_edges(std::make_pair(vertex, neighbour), true);
			callback_on_enter(neighbour);
			next_neighbour_to_visit[vertex]++;
		}
	}
}
