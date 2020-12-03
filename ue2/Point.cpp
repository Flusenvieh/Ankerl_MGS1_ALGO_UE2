#include "Point.h"

bool Point::operator<(const Point p) const
{
    return x < p.x || (x == p.x && y < p.y);
}

bool Point::operator>(const Point p) const
{
    return x > p.x || (x == p.x && y > p.y);
}