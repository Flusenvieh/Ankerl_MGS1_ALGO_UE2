#pragma once
#include <vector>

#include "Point.h"

class Quicksort
{
public:
	std::vector<Point> quicksort(std::vector<Point> list);
	void sort(Point* list, int l, int r);
};

