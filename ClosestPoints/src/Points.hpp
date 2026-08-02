#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>

class Point
{
private:
    double x, y;

public:
    Point() : x(0), y(0) {};
    Point(double a, double b) : x(a), y(b) {};
    double getX() const { return x; }
    double getY() const { return y; }
};

inline bool compareByX(const Point &a, const Point &b)
{
    return a.getX() < b.getX();
}
inline bool compareByY(const Point &a, const Point &b)
{
    return a.getY() < b.getY();
}

double distance(const Point &a, const Point &b);

void generateRandomPoints(std::vector<Point> &points, int numPoints, double max);
