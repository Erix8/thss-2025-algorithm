#include "Test.hpp"

void Find_Nearest_Points(std::vector<Point> &pointslist)
{
    double time = 0;
    double midDist = 0;

    // 先对整个序列进行排序
    SortListByX(pointslist);

    // 测试暴力搜索算法：
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Finding method: Brutual Finding" << std::endl;
    if (pointslist.size() < 100000)
    {
        auto start = std::chrono::high_resolution_clock::now();
        midDist = Brutual_Finding(pointslist, 0, pointslist.size());
        auto end = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Answer: " << midDist << std::endl;
        std::cout << "Time cost: " << time << "ms" << std::endl;
    }
    else
        std::cout << "Too Large Scale for Brutual Finding" << std::endl;

    std::cout << "-------------------------------------" << std::endl;

    // 测试改进版本算法：
    std::cout << "Finding method: Advanced Finding" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    midDist = Advanced_Finding(pointslist, 0, pointslist.size());
    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Answer: " << midDist << std::endl;
    std::cout << "Time cost: " << time << "ms" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}
void Generate_Test_Data(std::vector<Point> &pointslist)
{
    int total, range;
    std::cout << "Total Number of Dots: ";
    std::cin >> total;
    std::cout << "Range of the Dots: ";
    std::cin >> range;
    generateRandomPoints(pointslist, total, range);
}
void Read_Test_Data(std::vector<Point> &pointslist)
{
    std::string filename;
    std::cout << "Read from: ";
    std::cin >> filename;
    readPointsFromFile(pointslist, filename);
}
void Write_Test_Data(std::vector<Point> &pointslist)
{
    std::string filename;
    std::cout << "Write to: ";
    std::cin >> filename;
    writePointsToFile(pointslist, filename);
}
void Run_Finding_Test()
{
    std::vector<Point> pointList;
    int ins = 0;
    std::cout << "Welcome to Finding Test Program!" << std::endl;
    std::cout << "1. Generate Random Points and Run the Test." << std::endl;
    std::cout << "2. Read Points from File and Run the Test." << std::endl;
    std::cout << "3. Generate Random Points and Save to File." << std::endl;
    std::cout << "Your Instruction: ";
    std::cin >> ins;
    switch (ins)
    {
    case 1:
        Generate_Test_Data(pointList);
        Find_Nearest_Points(pointList);
        break;
    case 2:
        Read_Test_Data(pointList);
        Find_Nearest_Points(pointList);
        break;
    case 3:
        Generate_Test_Data(pointList);
        Write_Test_Data(pointList);
        break;
    default:
        break;
    }
    std::cout << "GoodBye!" << std::endl;
}