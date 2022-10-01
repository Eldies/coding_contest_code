#pragma once
#include <vector>


class DisjointSetUnion {
// O(1) on average for every request (well, O(inverse akkerman function(n)), but thats effectively O(1))  
	std::vector<size_t> parents;
	std::vector<size_t> ranks;
	std::vector<size_t> sizes;
public:
	DisjointSetUnion(size_t n) :
		ranks(n, 0),
		parents(n, -1),
		sizes(n, 1)
	{}

	size_t find_set(size_t v) {
		if (parents[v] == -1)
			return v;
		return parents[v] = find_set(parents[v]);
	}

	bool join_sets(size_t a, size_t b) {
		a = find_set(a);
		b = find_set(b);
		if (a != b) {
			if (ranks[a] < ranks[b])
				std::swap(a, b);
			parents[b] = a;
			if (ranks[a] == ranks[b])
				++ranks[a];
			sizes[a] = sizes[a] + sizes[b];
		}
		return (a != b);
	}

	size_t get_set_size(size_t v) {
		return sizes[find_set(v)];
	}
};
