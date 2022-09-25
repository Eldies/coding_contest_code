#pragma once
#include <functional>


double minimum_ternary_search(double left, double right, std::function<double(double)> func, double precision = 1e-7) {
	const static double golden_ratio = (std::sqrt(5) + 1) / 2;

	auto is_error_small_enough = [&left, &right, precision]() {
		auto absolute_error = right - left;
		auto max_border = std::max(std::abs(left), std::abs(right));
		return absolute_error / std::max(1., max_border) < precision;
	};

	double dist = right - left;

	double lx = right - dist / golden_ratio;
	double lf = func(lx);
	double rx = left + dist / golden_ratio;
	double rf = func(rx);

	while (!is_error_small_enough()) {
		if (lf < rf) {
			right = rx;
			rx = lx;
			rf = lf;
			lx = right - (right - left) / golden_ratio;
			lf = func(lx);
		}
		else {
			left = lx;
			lx = rx;
			lf = rf;
			rx = left + (right - left) / golden_ratio;
			rf = func(rx);
		}
	}

	return (right + left) / 2;
}
