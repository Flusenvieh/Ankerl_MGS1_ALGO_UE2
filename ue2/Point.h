#pragma once
class Point
{
    public:
        float x, y;
        Point() { x = 0.0f; y = 0.0f; }
        Point(float x, float y) :x(x), y(y){}
    
        bool operator<(const Point p) const;
        bool operator>(const Point p) const;
};

