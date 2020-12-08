#include "Andrews.h"
#include <iostream> //TODO: DELETE LATER

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
	if (_currentStatus < done && !_allPoints.empty())
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

	auto startCreateHull = std::chrono::high_resolution_clock::now();
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
	for (int i = n - 2; i >= 0; --i)//build lower hull
	{
		while (hullSize >= upperHullSize && cross(list[i], hull[hullSize - 1], hull[hullSize - 2]) < 0)
		{
			hullSize--;
		}
		hull[hullSize] = list[i];
		hullSize++;
	}

	hull.resize(hullSize - 1);

	auto stopCreateHull = std::chrono::high_resolution_clock::now();

	auto sortingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stopSorting - startSorting);
	auto creatingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stopCreateHull - startCreateHull);

	std::cout << "Single Thread\n";
	std::cout << "Presorting the Points took " << sortingDuration.count() << "s. \n";
	std::cout << "Creating the Hull took " << creatingDuration.count() << "s. \n";

	return hull;
}

std::vector<Point> Andrews::returnConvexHull_Multithreaded(std::vector<Point> list) const
{
	auto startSorting = std::chrono::high_resolution_clock::now();
	Quicksort qs;
	qs.sort(list.data(), 0, list.size() - 1);
	auto stopSorting = std::chrono::high_resolution_clock::now();

	auto startCreateHull = std::chrono::high_resolution_clock::now();
	int n = list.size();
	std::vector<Point> upperHullVector(n);
	std::vector<Point> lowerHullVector(n);
	std::vector<Point> returnHull;

	std::thread upperHull([this](std::vector<Point> list, std::vector<Point> &upperHullVector, int n)
		{
			int hullSize = 0;
			for (int i = 0; i < n; ++i)
			{
				while (hullSize >= 2 && cross(list[i], upperHullVector[hullSize - 1], upperHullVector[hullSize - 2]) < 0)
				{
					hullSize--;
				}
				upperHullVector[hullSize] = list[i];
				hullSize++;
			}
			upperHullVector.resize(hullSize);
		}
	, list, std::ref(upperHullVector), n);

	std::thread lowerHull([this](std::vector<Point> list, std::vector<Point> &lowerHullVector, int n)
		{
			int hullSize = 0;
			for (int i = 0; i < n; ++i)
			{
				while (hullSize >= 2 && cross(list[i], lowerHullVector[hullSize - 1], lowerHullVector[hullSize - 2]) > 0)
				{
					hullSize--;
				}
				lowerHullVector[hullSize] = list[i];
				hullSize++;
			}
			lowerHullVector.resize(hullSize);
			lowerHullVector.erase(lowerHullVector.begin());
			lowerHullVector.pop_back();
		}
	, list, std::ref(lowerHullVector), n);

	upperHull.join();
	lowerHull.join();

	returnHull.insert(returnHull.end(), upperHullVector.begin(), upperHullVector.end());
	returnHull.insert(returnHull.end(), lowerHullVector.rbegin(), lowerHullVector.rend());

	auto stopCreateHull = std::chrono::high_resolution_clock::now();


	auto sortingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stopSorting - startSorting);
	auto creatingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stopCreateHull - startCreateHull);

	std::cout << "\nMulti-Thread\n";
	std::cout << "Presorting the Points took " << sortingDuration.count() << "s. \n";
	std::cout << "Creating the Hull took " << creatingDuration.count() << "s. \n";

	return returnHull;
}