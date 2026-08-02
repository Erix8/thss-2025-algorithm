#pragma once
#include "Points.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

extern void writePointsToFile(const std::vector<Point> &points, const std::string &filename);
extern void readPointsFromFile(std::vector<Point> &points, const std::string &filename);