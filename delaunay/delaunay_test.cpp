#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../lib/tetgen.h"
#include "delaunay.h"

int main()
{
    srand(static_cast<unsigned>(time(0))); // 初始化随机数种子
    const int numPoints = 40;
    std::vector<double> points(numPoints * 3); // 每个点有x, y, z三个坐标

    // 生成40个随机点
    for (int i = 0; i < numPoints; ++i)
    {
        points[i * 3] = static_cast<double>(rand()) / RAND_MAX;     // x坐标
        points[i * 3 + 1] = static_cast<double>(rand()) / RAND_MAX; // y坐标
        points[i * 3 + 2] = static_cast<double>(rand()) / RAND_MAX; // z坐标
    }

    int outNumTetrahedra = 0;
    int *tetrahedra = delaunay(points.data(), numPoints, &outNumTetrahedra);

    // 输出生成的四面体数目及其点的索引
    std::cout << "生成的四面体数量: " << outNumTetrahedra << std::endl;
    for (int i = 0; i < outNumTetrahedra; ++i)
    {
        std::cout << "四面体 " << i + 1 << ": ";
        for (int j = 0; j < 4; ++j)
        {
            std::cout << tetrahedra[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }

    free(tetrahedra); // 释放动态分配的四面体数组
    return 0;
}
