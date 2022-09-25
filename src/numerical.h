#pragma once
#include <functional>


double minimum_ternary_search(double left, double right, std::function<double(double)> func, double precision = 1e-7) {
	// finds value in [left, right] which minimizes func
	// func should have non negative derivative
	// O(log((right-left)/precision))

	const static double golden_ratio = (std::sqrt(5) + 1) / 2;

	auto is_error_small_enough = [&left, &right, precision]() {
		auto absolute_error = right - left;
		auto max_border = std::max(std::abs(left), std::abs(right));
		return absolute_error / std::max(1., max_border) < precision;
	};

	double dist = right - left;

	struct MiddlePoint {
		double x;
		double y;
	};
	auto make_middle_point = [&func](double x) {return MiddlePoint{ x, func(x) }; };

	auto left_mp = make_middle_point(right - dist / golden_ratio);
	auto right_mp = make_middle_point(left + dist / golden_ratio);

	while (!is_error_small_enough()) {
		if (left_mp.y < right_mp.y) {
			right = right_mp.x;
			right_mp = left_mp;
			left_mp = make_middle_point(right - (right - left) / golden_ratio);
		}
		else {
			left = left_mp.x;
			left_mp = right_mp;
			right_mp = make_middle_point(left + (right - left) / golden_ratio);
		}
	}

	return (right + left) / 2;
}
