#pragma once

#include <vector>
#include <chrono>

#include "Point.h"
#include "Quicksort.h"

class Andrews
{
	public:
		Andrews() {}
		std::vector<Point> returnConvexHull(std::vector<Point> list) const;
	private:
		float cross(const Point O, const Point A, const Point B) const;
};

