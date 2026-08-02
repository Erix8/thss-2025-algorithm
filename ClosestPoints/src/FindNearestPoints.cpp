#include "FindNearestPoints.hpp"
void SortListByX(std::vector<Point> &points)
{
    std::sort(points.begin(), points.end(), compareByX);
}

void SortListByY(std::vector<Point> &points)
{
    std::sort(points.begin(), points.end(), compareByY);
}

double Brutual_Finding(const std::vector<Point> &points, int left, int right)
{
    double minDist = std::numeric_limits<double>::max();
    for (int i = left; i < right; i++)
        for (int j = i + 1; j < right; j++)
        {
            double tmp = distance(points[i], points[j]);
            minDist = std::min(minDist, tmp);
        }
    return minDist;
}

double Advanced_Finding(const std::vector<Point> &points, int left, int right)
{
    double minDist;
    // 当点数较少时，采用暴力算法计算即可
    if (right - left < 20)
    {
        minDist = Brutual_Finding(points, left, right);
        return minDist;
    }

    // 递归地解决左半部分和右半部分
    int mid = left + (right - left) / 2;
    double dLeft = Advanced_Finding(points, left, mid);
    double dRight = Advanced_Finding(points, mid, right);
    minDist = std::min(dLeft, dRight);

    // 创建一个垂直于中间位置的狭长带
    std::vector<Point> middleArea;
    double midX = points[mid].getX();
    for (int i = left; i < right; i++)
        if (std::fabs(points[i].getX() - midX) <= minDist)
            middleArea.push_back(points[i]);

    // 按Y坐标对狭长带中的点进行排序
    SortListByY(middleArea);

    // 检查狭长带中是否存在较小的点对距离
    int len = middleArea.size();
    for (int i = 0; i < len; i++)
    {
        int l = std::min(i + 7, len);
        for (int j = i + 1; j < l; j++)
        {
            double dis = distance(middleArea[i], middleArea[j]);
            minDist = std::min(minDist, dis);
        }
    }
    return minDist;
}
