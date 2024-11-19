// delaunay.h
#ifndef DELAUNAY_H
#define DELAUNAY_H

#ifdef __cplusplus
extern "C" {
#endif

// Delaunay四面体剖分函数声明
// 参数:
// - points: 输入的点数组，类型为 double*
// - numPoints: 点的数量
// - outNumTetrahedra: 指向输出四面体数量的指针
// 返回值: 一个包含四面体顶点索引的int数组
int* delaunay(double* points, int numPoints, int* outNumTetrahedra);

#ifdef __cplusplus
}
#endif

#endif // DELAUNAY_H
