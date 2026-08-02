#pragma once
#include "Points.hpp"
#include <algorithm>
#include <limits>
#include <cmath>

void SortListByX(std::vector<Point> &points);

void SortListByY(std::vector<Point> &points);

double Brutual_Finding(const std::vector<Point> &points, int left, int right);

double Advanced_Finding(const std::vector<Point> &points, int left, int right);