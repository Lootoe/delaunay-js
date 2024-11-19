#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdio> // 使用 printf
#include <array>

// 计算四面体的外接球圆心并存储在 center 中
void tetCircumcenter(const double a[3], const double b[3], const double c[3], const double d[3], double center[3]) {
    double xba = b[0] - a[0], yba = b[1] - a[1], zba = b[2] - a[2];
    double xca = c[0] - a[0], yca = c[1] - a[1], zca = c[2] - a[2];
    double xda = d[0] - a[0], yda = d[1] - a[1], zda = d[2] - a[2];

    double balength = xba * xba + yba * yba + zba * zba;
    double calength = xca * xca + yca * yca + zca * zca;
    double dalength = xda * xda + yda * yda + zda * zda;

    double xcrosscd = yca * zda - yda * zca;
    double ycrosscd = zca * xda - zda * xca;
    double zcrosscd = xca * yda - xda * yca;

    double xcrossdb = yda * zba - yba * zda;
    double ycrossdb = zda * xba - zba * xda;
    double zcrossdb = xda * yba - xba * yda;

    double xcrossbc = yba * zca - yca * zba;
    double ycrossbc = zba * xca - zca * xba;
    double zcrossbc = xba * yca - xca * yba;

    double denominator = 0.5 / (xba * xcrosscd + yba * ycrosscd + zba * zcrosscd);

    center[0] = (balength * xcrosscd + calength * xcrossdb + dalength * xcrossbc) * denominator + a[0];
    center[1] = (balength * ycrosscd + calength * ycrossdb + dalength * ycrossbc) * denominator + a[1];
    center[2] = (balength * zcrosscd + calength * zcrossdb + dalength * zcrossbc) * denominator + a[2];
}

// orient3d 函数，用于确定四点排列顺序
double orient3d(const double a[3], const double b[3], const double c[3], const double d[3]) {
    double xba = b[0] - a[0], yba = b[1] - a[1], zba = b[2] - a[2];
    double xca = c[0] - a[0], yca = c[1] - a[1], zca = c[2] - a[2];
    double xda = d[0] - a[0], yda = d[1] - a[1], zda = d[2] - a[2];

    return xba * (yca * zda - zca * yda) - yba * (xca * zda - zca * xda) + zba * (xca * yda - yca * xda);
}

// 计算两点之间的距离
double distance(const double p1[3], const double p2[3]) {
    return std::sqrt(std::pow(p1[0] - p2[0], 2) + std::pow(p1[1] - p2[1], 2) + std::pow(p1[2] - p2[2], 2));
}


// 修改后的 calcTetRadius 函数，使用 std::vector<std::array<double, 3>> 数据结构
double calcTetRadius(const int* indexes, const std::vector<std::array<double, 3>>& points) {
    std::array<std::array<double, 3>, 4> vertices;
    for (int i = 0; i < 4; ++i) {
        int idx = indexes[i];
        vertices[i] = points[idx];
    }

    double center[3];
    double orient = orient3d(vertices[0].data(), vertices[1].data(), vertices[2].data(), vertices[3].data());
    if (orient < 0.0) {
        tetCircumcenter(vertices[0].data(), vertices[2].data(), vertices[1].data(), vertices[3].data(), center);
    } else {
        tetCircumcenter(vertices[0].data(), vertices[1].data(), vertices[2].data(), vertices[3].data(), center);
    }

    return distance(center, vertices[0].data());
}