#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <cstdio> // 使用 printf
#include "../delaunay/delaunay.h"

double calcTetRadius(const int* indexes, const std::vector<std::array<double, 3>>& points);

extern "C" {
    int* alphaShape(double* rawPoints, int numPoints, double alpha, int* outNumFaces) {
        // 将原始的 double* points 转换为 std::vector<std::array<double, 3>>
        std::vector<std::array<double, 3>> points(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            points[i] = { rawPoints[i * 3], rawPoints[i * 3 + 1], rawPoints[i * 3 + 2] };
        }

        // 调用 Delaunay 四面体剖分函数
        int numTetrahedra;
        int* delaunayTets = delaunay(rawPoints, numPoints, &numTetrahedra);

        std::map<std::vector<int>, int> faceCount;

        for (int i = 0; i < numTetrahedra; i++) {
            int tet[4] = {
                delaunayTets[i * 4 + 0],
                delaunayTets[i * 4 + 1],
                delaunayTets[i * 4 + 2],
                delaunayTets[i * 4 + 3]
            };

            // 计算四面体的外接球半径
            double radius = calcTetRadius(tet, points);
            if (radius <= alpha) {
                std::vector<std::vector<int>> faces = {
                    {tet[0], tet[1], tet[2]},
                    {tet[0], tet[1], tet[3]},
                    {tet[0], tet[2], tet[3]},
                    {tet[1], tet[2], tet[3]}
                };

                for (auto& face : faces) {
                    std::sort(face.begin(), face.end());
                    faceCount[face]++;
                }
            }
        }

        std::vector<std::vector<int>> boundaryFaces;
        for (const auto& it : faceCount) {
            if (it.second == 1) {
                boundaryFaces.push_back(it.first);
            }
        }

        *outNumFaces = boundaryFaces.size();
        int* result = (int*)malloc(boundaryFaces.size() * 3 * sizeof(int));
        int idx = 0;
        for (const auto& face : boundaryFaces) {
            for (int vertex : face) {
                result[idx++] = vertex;
            }
        }

        free(delaunayTets);
        return result;
    }
}