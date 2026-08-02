#include "FileIO.hpp"

void writePointsToFile(const std::vector<Point> &points, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open the file for writing: " << filename << std::endl;
        return;
    }
    for (const auto &p : points)
    {
        file << p.getX() << " " << p.getY() << "\n";
    }
    file.close();
    std::cout << "Writing complete" << std::endl;
}

void readPointsFromFile(std::vector<Point> &points, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Cannot open the file for reading: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        double x, y;
        if (iss >> x >> y)
        {
            Point p(x, y);
            points.push_back(p);
        }
    }
    file.close();
    std::cout << "Reading complete" << std::endl;
}