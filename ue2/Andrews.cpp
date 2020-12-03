#include "Andrews.h"

// Cross product of two vectors OA and OB 
// returns positive for counter clockwise 
// turn and negative for clockwise turn 
float Andrews::cross(const Point O, const Point A, const Point B) const
{
	return (A.x - O.x) * (B.y - O.y)
		- (A.y - O.y) * (B.x - O.x);
}

std::vector<Point> Andrews::returnConvexHull(std::vector<Point> list) const
{
	int n = list.size();

	//Sort list by x-coordinate, in case of tie use y-coordinate
	auto startSorting = std::chrono::high_resolution_clock::now();
	Quicksort qs;
	qs.sort(list.data(), 0, list.size() - 1);
	auto stopSorting = std::chrono::high_resolution_clock::now();

	std::vector<Point> hull;
	for (size_t i = 0; i < list.size(); ++i)//build upper hull
	{
		int hullSize = hull.size();
		while (hullSize >= 2 && cross(list[i], hull[hullSize - 1], hull[hullSize - 2]) < 0)
		{
			hull.pop_back();
			hullSize--;
		}
		hull.push_back(list[i]);
	}
	int upperHullSize = hull.size() + 1; // add one to avoid deleting turning point
	for (size_t i = list.size() - 2; i > 0; --i)//build upper hull
	{
		int hullSize = hull.size();
		while (hullSize >= upperHullSize && cross(list[i], hull[hullSize - 1], hull[hullSize - 2]) < 0)
		{
			hull.pop_back();
			hullSize--;
		}
		hull.push_back(list[i]);
	}
	hull.pop_back();
	hull.shrink_to_fit();
	return hull;
}
