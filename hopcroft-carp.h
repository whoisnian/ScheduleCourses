#ifndef HOPCROFTCARP_H
#define HOPCROFTCARP_H

#include<queue>
#include<cstring>
#include<cstdio>
int teachernum,coursenum;//老师的数量，课程的数量
int graph[105][105];//二分图的邻接矩阵存储
int teacher_result[105],course_result[105];//存储匹配结果
int visited[105];
int teacher_dis[105],course_dis[105];//存储增广路的距离
int mindis;
bool searchpath()//若返回 true,表示存在最短增广路径集
{
    std::queue<int>q;//使用stl内的队列
    while(!q.empty())q.pop();//清空
    memset(teacher_dis,-1,sizeof(teacher_dis));//初始化
    memset(course_dis,-1,sizeof(course_dis));
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
int findpath(int now)
{
    int i;
    for(i=0;i<coursenum;i++)
    {
        if(!visited[i]&&graph[now][i]&&course_dis[i]==teacher_dis[now]+1)//若该课程未访问且符合距离上的增广条件
        {
            visited[i]=1;
            if(course_result[i]!=-1&&course_dis[i]==mindis)continue;//若该课程已有老师上且已达到最短增广距离的长度，跳过该课程
            if(course_result[i]==-1||findpath(course_result[i]))//若该课程没有老师上或者找到了最短增广路经，记录匹配结果
            {
                course_result[i]=now;
                teacher_result[now]=i;
                return true;
            }
        }
    }
    return false;
}
int maxmatch()//类似匈牙利算法，返回值为最大匹配数
{
    int i,maxmatchnum=0;
    memset(teacher_result,-1,sizeof(teacher_result));//初始化
    memset(course_result,-1,sizeof(course_result));
    while(searchpath())//当存在增广路
    {
        memset(visited,0,sizeof(visited));//清空访问标记
        for(i=0;i<teachernum;i++)//对于每个老师，如果没有课，尝试找一个
        if(teacher_result[i]==-1)
            maxmatchnum+=findpath(i);//若成功匹配，答案加一
    }
    return maxmatchnum;
}
/*
int main()
{
    int tt,i,j,s,t;
    scanf("%d",&tt);
    while(tt--)
    {
        scanf("%d %d",&teachernum,&coursenum);
        memset(graph,0,sizeof(graph));
        for(i=0;i<teachernum;i++)
        {
            scanf("%d",&s);
            for(j=0;j<s;j++)
            {
                scanf("%d",&t);
                graph[i][t-1]=1;
            }
        }
        printf("%d\n",maxmatch());
    }
    return 0;
}*/

#endif // HOPCROFTCARP_H
