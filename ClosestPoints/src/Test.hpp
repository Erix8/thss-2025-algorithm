#pragma once
#include "FindNearestPoints.hpp"
#include "FileIO.hpp"
#include <chrono>

void Generate_Test_Data(std::vector<Point> &pointslist);
void Read_Test_Data(std::vector<Point> &pointslist);
void Write_Test_Data(std::vector<Point> &pointslist);
void Find_Nearest_Points(std::vector<Point> &pointslist);
void Run_Finding_Test();