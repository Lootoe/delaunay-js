#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "alphaShape.h"

// 生成随机点
std::vector<double> generateRandomPoints(int numPoints) {
    std::vector<double> points(numPoints * 3);
    for (int i = 0; i < numPoints * 3; ++i) {
        points[i] = static_cast<double>(rand()) / RAND_MAX * 10.0; // 随机生成0-10范围内的数
    }
    return points;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // 初始化随机数种子
    int numPoints = 100;
    double alpha = 10;

    // 生成随机的三维点
    std::vector<double> points = generateRandomPoints(numPoints);

    // 调用 alphaShape 函数
    int outNumFaces;
    int* result = alphaShape(points.data(), numPoints, alpha, &outNumFaces);

    // 输出结果
    std::cout << "符合 alpha = " << alpha << " 条件的边界面数量: " << outNumFaces << std::endl;
    std::cout << "边界面顶点索引：" << std::endl;
    for (int i = 0; i < outNumFaces; ++i) {
        std::cout << "Face " << i + 1 << ": ("
                  << result[i * 3] << ", "
                  << result[i * 3 + 1] << ", "
                  << result[i * 3 + 2] << ")" << std::endl;
    }

    // 释放内存
    free(result);

    return 0;
}
