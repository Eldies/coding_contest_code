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
