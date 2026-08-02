#include "Points.hpp"

double distance(const Point &a, const Point &b)
{
    double deltaX = a.getX() - b.getX();
    double deltaY = a.getY() - b.getY();
    double dis = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    return dis;
}

void generateRandomPoints(std::vector<Point> &points, int numPoints, double max)
{
    points.reserve(numPoints);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < numPoints; ++i)
    {
        double x = static_cast<double>(std::rand()) / RAND_MAX * max;
        double y = static_cast<double>(std::rand()) / RAND_MAX * max;
        Point p(x, y);
        points.push_back(p);
    }
    std::cout << "Generating Complete!" << std::endl;
}
