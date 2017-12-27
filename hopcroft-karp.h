#ifndef HOPCROFTKARP_H
#define HOPCROFTKARP_H

#include <vector>
#include "queue.h"
#include <set>
#include <string>
#include <cstring>

class HopcroftKarp
{
public:
    HopcroftKarp(std::vector< std::vector<std::string> *> courses, std::set<std::string> courseslist);
    int maxmatch(); //类似匈牙利算法，返回值为最大匹配数
    int *result(); //获取存放结果的数组

private:
    int **graph; //二分图的邻接矩阵存储
    int teachernum, coursenum; //老师的数量，课程的数量
    int *teacher_dis, *course_dis;//存储增广路的距离
    int *teacher_result, *course_result; //存储匹配结果
    int mindis;

    bool searchpath(); //若返回 true,表示存在最短增广路径集
    int findpath(int now, int *visited);
};

#endif // HOPCROFTKARP_H
