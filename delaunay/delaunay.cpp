#include "../lib/tetgen.h"
#include <cstdio> // 使用 printf

// 封装TetGen的Delaunay四面体剖分函数
extern "C" {
    int* delaunay(double* points, int numPoints, int* outNumTetrahedra) {
        tetgenio in, out;
        in.numberofpoints = numPoints;
        in.pointlist = points;

        // TetGen行为设置
        tetgenbehavior behavior;
        behavior.quiet = 1;   // 静默模式
        behavior.plc = 0;     // 不保持输入的平面约束
        behavior.quality = 0; // 不进行网格质量优化

        // 执行Delaunay四面体剖分
        tetrahedralize(&behavior, &in, &out);

        // 提取四面体
        *outNumTetrahedra = out.numberoftetrahedra;

        // 分配内存以保存四面体的顶点索引，每个四面体4个顶点
        int* tetrahedra = (int*)malloc(out.numberoftetrahedra * 4 * sizeof(int));
        for (int i = 0; i < out.numberoftetrahedra * 4; i++) {
            tetrahedra[i] = out.tetrahedronlist[i];
        }

        return tetrahedra;
    }
}
