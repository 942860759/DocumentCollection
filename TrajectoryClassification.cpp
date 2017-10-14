//
//  main.cpp
//  TrajectoryClassification
//
//  Created by 唐伟 on 2017/10/10.
//  Copyright © 2017年 唐伟. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;

#define MAX 200
#define pi 3.1415926
const int M=5;// 全部M个人轨迹
const double w0=pi/2;//转角阈值
const double e0=0.5;//近邻阈值
const double WD=0.25,WA=0.25,WL=0.5;//方向、速度WS=0、转角、位置的权重
const int MinLns=1;//近邻数量阈值
double maxSDIDT;//全局变量

struct Point{
    int x;
    int y;
};
struct Trajectory{
    int n;// 走了n步
    Point point[MAX];
}trajectory[M]={
    {7,{{1,1},{1,2},{3,3},{4,2},{5,6},{7,8},{8,9}}},
    {10,{{5,2},{2,3},{6,4},{4,5},{5,8},{7,9},{10,9},{12,13},{13,14},{16,13}}},
    {6,{{4,5},{8,3},{3,4},{9,5},{5,6},{7,8}}},
    {7,{{4,8},{7,3},{3,4},{4,0},{5,6},{8,8},{8,9}}},
    {8,{{2,5},{4,8},{7,4},{4,3},{5,6},{7,9},{9,9},{11,6}}},
};
const int MaxLength=50;
struct Orbit{
    Point pointF;
    Point pointL;
    int attribute;
}orbit[MaxLength];//轨迹段集合TS

int orbitNum[MaxLength]={0};//轨迹段条数

Trajectory trajectorySection[M];//划分后得到轨迹分段，这里是初始定义

//返回两个数中最小的数
double min_2N(double x,double y){
    if(x<y)
        return x;
    else
        return y;
}
//s点到以a，b为端点的直线距离
double dis_2D(Point a,Point b,Point s){
    double ab=sqrt(pow((a.x-b.x),2.0)+pow((a.y-b.y),2.0));
    double as=sqrt(pow((a.x-s.x),2.0)+pow((a.y-s.y),2.0));
    double bs=sqrt(pow((s.x-b.x),2.0)+pow((s.y-b.y),2.0));
    
    double cos_A=(pow(as,2.0)+pow(ab,2.0)-pow(bs,2.0))/(2*ab*as);
    double sin_A=sqrt(1-pow(cos_A,2.0));
    return as*sin_A;
}
//s点到以a，b为端点的直线的垂点距离端点最小距离
double dis_2V(Point a,Point b,Point s){
    double ab=sqrt(pow((a.x-b.x),2.0)+pow((a.y-b.y),2.0));
    double as=sqrt(pow((a.x-s.x),2.0)+pow((a.y-s.y),2.0));
    double bs=sqrt(pow((s.x-b.x),2.0)+pow((s.y-b.y),2.0));
    
    double cos_A=(pow(as,2.0)+pow(ab,2.0)-pow(bs,2.0))/(2*ab*as);
    if(cos_A<0)
        return -as*cos_A;
    else{
        if(as*cos_A>ab)
            return as*cos_A-ab;
        else
            return min_2N(as*cos_A,ab-as*cos_A);
    }
}
//计算轨迹转角
int triangleAngle(Point point1,Point point2,Point point3){
    double a,b,c,m0,m1;
    int flag;
    a=sqrt((point2.x-point1.x)*(point2.x-point1.x)+(point2.y-point1.y)*(point2.y-point1.y));
    b=sqrt((point3.x-point2.x)*(point3.x-point2.x)+(point3.y-point2.y)*(point3.y-point2.y));
    c=sqrt((point3.x-point1.x)*(point3.x-point1.x)+(point3.y-point1.y)*(point3.y-point1.y));
    m0=acos((a*a+b*b-c*c)/(2*a*b));
    flag=(point2.x-point1.x)*(point3.x-point2.x)+(point2.y-point1.y)*(point3.y-point2.y);
    if(flag>=0){
        m1=pi-m0;
    }else{
        m1=m0-pi;
    }
    return m1;
}
//划分轨迹为轨迹分段(转角大于90度的点为特征点)
void trackSegment(){
    int flag;
    int m;
    for(int i=0;i<M;i++){
        //每个轨迹有0到n-3个转角
        m=0;
        trajectorySection[i].point[m].x=trajectory[i].point[0].x;
        trajectorySection[i].point[m].y=trajectory[i].point[0].y;
        m=m+1;
        for(int j=0;j<trajectory[i].n-2;j++){
            flag=triangleAngle(trajectory[i].point[j],trajectory[i].point[j+1],trajectory[i].point[j+2]);
            if(fabs(flag)>=w0){
                trajectorySection[i].point[m].x=trajectory[i].point[j+1].x;
                trajectorySection[i].point[m].y=trajectory[i].point[j+1].y;
                m++;
            }
        }
        trajectorySection[i].point[m].x=trajectory[i].point[trajectory[i].n-1].x;
        trajectorySection[i].point[m].y=trajectory[i].point[trajectory[i].n-1].y;
        trajectorySection[i].n=m+1;
    }
}
//划分轨迹转换为轨迹段集合形式TS
int trackTurn(){
    int m=0;
    for(int i=0;i<M;i++){
        for(int j=0;j<trajectorySection[i].n-1;j++){
            orbit[m].pointF.x=trajectorySection[i].point[j].x;
            orbit[m].pointF.y=trajectorySection[i].point[j].y;
            orbit[m].pointL.x=trajectorySection[i].point[j+1].x;
            orbit[m].pointL.y=trajectorySection[i].point[j+1].y;
            orbit[m].attribute=i;
            m++;
        }
        if(i>=1)
            orbitNum[i]=m-orbitNum[i-1];
        else
            orbitNum[i]=m;
    }
    return m;
}
//根据权重计算轨迹段的结构相似度
double DirDist(Orbit orbit1,Orbit orbit2){
    int dirDist;
    double m,l,n,i,j,temp;
    m=sqrt(orbit1.pointF.x*orbit1.pointF.x+orbit1.pointF.y*orbit1.pointF.y);
    l=sqrt(orbit1.pointL.x*orbit1.pointL.x+orbit1.pointL.y*orbit1.pointL.y);
    n=acos((orbit1.pointF.x*orbit1.pointL.x+orbit1.pointF.y*orbit1.pointL.y)/(m*l));
    i=(orbit1.pointF.x-orbit1.pointL.x)*(orbit1.pointF.x-orbit1.pointL.x)+(orbit1.pointF.y-orbit1.pointL.y)*(orbit1.pointF.y-orbit1.pointL.y);
    j=(orbit2.pointF.x-orbit2.pointL.x)*(orbit2.pointF.x-orbit2.pointL.x)+(orbit2.pointF.y-orbit2.pointL.y)*(orbit2.pointF.y-orbit2.pointL.y);
    temp=sqrt(min_2N(i, j));
    if(0<=n&&n<=(pi/2)){
        dirDist=temp*sin(n);
    }else if((pi/2)<=n&&n<=pi){
        dirDist=temp;
    }else{
        dirDist=0;
    }
    return dirDist;//角度
}
double AngleDist(Orbit orbit1,Orbit orbit2){
    double angleDist1,angelDist2;
    double l1,l2,m1,m2;
    l1=dis_2V(orbit1.pointF,orbit1.pointL,orbit2.pointF);
    l2=dis_2V(orbit1.pointF,orbit1.pointL,orbit2.pointL);
    angleDist1=min_2N(l1, l2);
    m1=dis_2V(orbit2.pointF,orbit2.pointL,orbit1.pointF);
    m2=dis_2V(orbit2.pointF,orbit2.pointL,orbit1.pointL);
    angelDist2=min_2N(m1, m2);
    return min_2N(angleDist1, angelDist2);//转角
}
double LocDist(Orbit orbit1,Orbit orbit2){
    double locDist1,locDist2;
    double l1,l2,m1,m2;
    l1=dis_2D(orbit1.pointF,orbit1.pointL,orbit2.pointF);
    l2=dis_2D(orbit1.pointF,orbit1.pointL,orbit2.pointL);
    locDist1=(l1*l1+l2*l2)/(l1+l2);
    m1=dis_2D(orbit2.pointF,orbit2.pointL,orbit1.pointF);
    m2=dis_2D(orbit2.pointF,orbit2.pointL,orbit1.pointL);
    locDist2=(m1*m1+m2*m2)/(m1+m2);
    return min_2N(locDist1,locDist2);//位置
}

int SDIST(Orbit orbit1,Orbit orbit2){
    double dirDist,angleDist,locDist;//SpeedDist权重为0
    double SDIST;
    dirDist=DirDist(orbit1,orbit2);
    angleDist=AngleDist(orbit1,orbit2);
    locDist=LocDist(orbit1,orbit2);
    SDIST=WD*dirDist+WA*angleDist+WL*locDist;
    return SDIST;
}

int SSIM(Orbit orbit1,Orbit orbit2){
    return 1-SDIST(orbit1,orbit2)/maxSDIDT;//归一化函数,SSIM越大，越相似
}

void Algorithm(){
    //基于结构相似度的轨迹聚类算法_袁冠
    //算法：Trajectory Clustering based on Structural Similarity (TC-SS)
    //输入：轨迹集合I={TR1, TR2, …, TRn }，相关参数：ω（转角阈值），W（权重向量），ε（近邻阈值），σ（近邻数量阈值）
    //输出：轨迹段聚类集合O={C1, C2, …, Cnum}
    /*第1阶段：轨迹段相似度计算*/
    //1) for each TRi ∈ I  do
    //2)   TS←根据ω划分TRi;
    //3) for each Li, Lj ∈ TS ∧ i≠j ∧Li.TR ≠ Lj.TR do
    //4)   Initial SSIM(Li, Lj);   /*根据权重计算轨迹段的结构相似度*/
    //5)   SSIM_Computer(Li, Lj)* W; /*计算轨迹段间的结构相似度*/
    //6)   if SSIM(Li, Lj)≥ε then
    //7)     Nε(Li)←Lj;  /*设定Li 的ε-近邻集合*/
    //end for
    /*第2阶段：轨迹段进行聚类*/
    //8) Set clusterId = 0;
    //9) Mark all segments as unclassified;
    //10) for each Li ∈ TS and Li is unclassified do
    //11)   if | Nε(Li) | ≥ σ then /*寻找可作为聚类中心的轨迹段*/
    //12)     for each Lj ∈ Nε(Li) do /*遍历轨迹ε近邻*/
    //13)       if Rdis(Li, Lj) is φ or Cdis(Li) is φ then
    //14)          Mark Li as noise;
    //else  /*对设置聚类ID*/
    //15)          Assign clusterId to ∀L ∈ Nε(Li);
    //16)          Add Nε(Li)- Li to Queue Q;
    //17)          ExpandCluster(Q, clusterId, σ); /*扩展轨迹的近邻聚类*/
    //end for
    //18)    Increase clusterId by 1;
    //end for;  /*对可达近邻进行聚类*/
    //19) ExpandCluster(Q, clusterId, σ)
    //20)   while (Q≠φ) do /*遍历队列中的轨迹段*/
    //21)     Get L from Q;
    //22)     if | Nε(L) | ≥ σ then
    //23)       for each X ∈ Nε(L) do
    //24)         if X is noise then
    //25)           Assign clusterId to X;
    //else
    //26)            Add X to Queue Q;
    //27)     Remove L from Queue Q; End
    
    trackSegment();
    int lengthN=trackTurn();
    double maxTempSDIST=0;
    double tempSD,tempSS;
    
    for(int i=0;i<lengthN;i++){
        for(int j=i+1;j<lengthN;j++){
            tempSD=SDIST(orbit[i],orbit[j]);
            if(tempSD>maxTempSDIST)
                maxTempSDIST=tempSD;
        }
    }
    maxSDIDT = maxTempSDIST;//归一化要求最大值
    
    Orbit orbitNE[lengthN][lengthN];
    for(int i=0;i<lengthN;i++){
        for(int j=i+1;j<lengthN;j++){
            tempSS=SSIM(orbit[i],orbit[j]);
            if(tempSS>e0)
                orbitNE[i][j]=orbit[j];
        }
    }
    
}

int main(int argc, const char * argv[]) {

    Algorithm();
    
    
    
    // insert code here...
    cout << "Hello, World!\n"<<endl;
    return 0;
}
