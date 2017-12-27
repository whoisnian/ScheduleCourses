#include "hopcroft-karp.h"
#include <map>

HopcroftKarp::HopcroftKarp(std::vector<std::vector<std::string> *> courses, std::set<std::string> courseslist)
{
    teachernum = courses.size();
    coursenum = courseslist.size();

    graph = new int*[teachernum + 1];
    for(int i = 0;i <= teachernum;i++)
        graph[i] = new int[coursenum + 1];
    for(int i = 0;i <= teachernum;i++)
        for(int j = 0;j <= coursenum;j++)
            graph[i][j] = 0;
    teacher_dis = new int[teachernum + 1];
    course_dis = new int[coursenum + 1];
    teacher_result = new int[teachernum + 1];
    course_result = new int[coursenum + 1];

    std::map<std::string, int> m;
    int i = 0;
    for(std::set<std::string>::iterator it = courseslist.begin();it != courseslist.end();it++, i++)
        m[*it] = i;
    for(int i = 0;i < teachernum;i++)
    {
        for(int j = 0;j < int(courses.at(i)->size());j++)
        {
            graph[i][m[courses.at(i)->at(j)]] = 1;
        }
    }
}

bool HopcroftKarp::searchpath()
{
    queue<int>q;//使用队列
    while(!q.empty())q.pop();//清空
    memset(teacher_dis,-1,sizeof(int)*(teachernum+1));//初始化
    memset(course_dis,-1,sizeof(int)*(coursenum+1));
    mindis=99999;//相当于无穷大
    int i;
    for(i=0;i<teachernum;i++)//所有还没有课上的老师入队
    if(teacher_result[i]==-1)
    {
        q.push(i);
        teacher_dis[i]=0;
    }
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        if(teacher_dis[now]>mindis)break;//路径长度大于已经找到的最短增广路经长度，退出
        for(i=0;i<coursenum;i++)
        {
            if(graph[now][i]&&course_dis[i]==-1)//如果存在边
            {
                course_dis[i]=teacher_dis[now]+1;//更新该课程在增广路径上的距离
                if(course_result[i]==-1)
                    mindis=course_dis[i];//发现增广路经，记录最短增广路经长度
                else
                {
                    teacher_dis[course_result[i]]=course_dis[i]+1;//更新距离，继续寻找
                    q.push(course_result[i]);
                }
            }
        }
    }
    if(mindis!=99999)return true;
    else return false;
}

int HopcroftKarp::findpath(int now, int *visited)
{
    int i;
    for(i=0;i<coursenum;i++)
    {
        if(!visited[i]&&graph[now][i]&&course_dis[i]==teacher_dis[now]+1)//若该课程未访问且符合距离上的增广条件
        {
            visited[i]=1;
            if(course_result[i]!=-1&&course_dis[i]==mindis)continue;//若该课程已有老师上且已达到最短增广距离的长度，跳过该课程
            if(course_result[i]==-1||findpath(course_result[i], visited))//若该课程没有老师上或者找到了最短增广路经，记录匹配结果
            {
                course_result[i]=now;
                teacher_result[now]=i;
                return true;
            }
        }
    }
    return false;
}

int HopcroftKarp::maxmatch()
{
    int i,maxmatchnum=0;
    int *visited = new int[coursenum + 1];
    memset(teacher_result,-1,sizeof(int)*(teachernum+1));//初始化
    memset(course_result,-1,sizeof(int)*(coursenum+1));
    while(searchpath())//当存在增广路
    {
        memset(visited,0,sizeof(int)*(coursenum+1));//清空访问标记
        for(i=0;i<teachernum;i++)//对于每个老师，如果没有课，尝试找一个
        if(teacher_result[i]==-1)
            maxmatchnum+=findpath(i, visited);//若成功匹配，答案加一
    }
    return maxmatchnum;
}

int *HopcroftKarp::result()
{
    return teacher_result;
}
