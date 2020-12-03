#include "Andrews.h"

void Andrews::InitSteppable(std::vector<Point> list)
{
	_allPoints = list;
	_hullPoints.resize(list.size() * 2);

	Reset();

	//Sort list by x-coordinate, in case of tie use y-coordinate
	Quicksort qs;
	qs.sort(_allPoints.data(), 0, _allPoints.size() - 1);
}

std::vector<Point> Andrews::Step(void)
{
	int hullSize;
	switch (_currentStatus)//build upper hull
	{
		case upper:
			hullSize = _hullPoints.size();
			if (hullSize >= 3 && cross(_hullPoints[hullSize - 1], _hullPoints[hullSize - 2], _hullPoints[hullSize - 3]) < 0)
			{
				_hullPoints.erase(_hullPoints.end()-2);
			}
			else
			{		
				if (_i == _allPoints.size())
				{
					_currentStatus = lower;
					_i = _allPoints.size() - 2;
					_upperHullSize = hullSize + 1;
					return _hullPoints;
				}
				_hullPoints.push_back(_allPoints[_i]);
				_i++;
			}

			break;
		case lower:

			hullSize = _hullPoints.size();
			if (hullSize >= _upperHullSize && cross(_hullPoints[hullSize - 1], _hullPoints[hullSize - 2], _hullPoints[hullSize - 3]) < 0)
			{
				_hullPoints.erase(_hullPoints.end() - 2);
			}
			else
			{
				if (_i < 0)
				{
					_currentStatus = done;
					_hullPoints.pop_back();
					_hullPoints.shrink_to_fit();
					return _hullPoints;
				}
				_hullPoints.push_back(_allPoints[_i]);
				_i--;
			}

			break;
	}
	return _hullPoints;
}

std::vector<Point> Andrews::State(void) const
{
	return _hullPoints;
}

bool Andrews::HasNextStep(void) const
{
	if (_currentStatus < done)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Andrews::Reset(void)
{	
	_hullPoints.clear();

	_i = 0;
	_upperHullSize = 0;
	_currentStatus = upper;
}

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

	std::vector<Point> hull(2*n);
	int hullSize = 0;
	for (int i = 0; i < n; ++i)//build upper hull
	{
		while (hullSize >= 2 && cross(list[i], hull[hullSize - 1], hull[hullSize - 2]) < 0)
		{
			hullSize--;
		}
		hull[hullSize] = list[i];
		hullSize++;
	}

	int upperHullSize = hullSize + 1; // add one to avoid deleting turning point
	for (int i = n - 2; i > 0; --i)//build lower hull
	{
		while (hullSize >= upperHullSize && cross(list[i], hull[hullSize - 1], hull[hullSize - 2]) < 0)
		{
			hullSize--;
		}
		hull[hullSize] = list[i];
		hullSize++;
	}

	hull.resize(hullSize - 1);
	return hull;
}
