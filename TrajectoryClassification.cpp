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
const int M = 8;// 全部M个人轨迹
const double w0 = pi / 9;//转角阈值
const double e0 = 0.75;//近邻阈值
const double WD = 0.25, WA = 0.25, WL = 0.5;//方向、速度WS=0、转角、位置的权重
const int MinLns = 1;//近邻数量阈值
double maxSDIDT;//全局变量

struct Point {
    int x;
    int y;
};
struct Trajectory {
    int n;// 走了n步
    Point point[MAX];
}trajectory[M] = {
    {32,{{3,125 },{5,123 },{22,117 },{33,112 },{49,104 },{66,98 },{74,95 },{77,95 },{89,92 },{100,90 },{111,86 },{121,82 },{135,75 },{150,69 },{165,57 },{176,48 },{183,42 },{192,36 },{198,32 },{204,29 },{218,22 },{227,18 },{236,18 },{244,18 },{256,18 },{265,18 },{283,14 },{288,13 },{292,12 },{301,7 },{308,2 },{316,-4}}},
    {42,{{1,98 },{4,98 },{20,98 },{37,98 },{48,97 },{55,96 },{59,98 },{63,99 },{73,99 },{86,100 },{103,100 },{107,98 },{116,94 },{126,89 },{139,86 },{149,82 },{155,82 },{159,80 },{166,76 },{173,71 },{177,69 },{189,64 },{193,61 },{206,58 },{225,54 },{230,53 },{238,50 },{243,49 },{257,46 },{264,46 },{270,46 },{284,43 },{306,37 },{318,35 },{322,34 },{335,29 },{349,22 },{356,19 },{362,15 },{373,6 },{382,2 },{385,0}}},
    {44,
        {{6,171 },{20,171 },{29,171 },{41,168 },{62,161 },{69,159 },{84,152 },{96,143 },{107,136 },{122,127 },{130,122 },{146,111 },{156,104 },{167,94 },{178,85 },{184,81 },{188,78 },{194,74 },{202,70 },{206,68 },{213,64 },{220,61 },{224,61 },{236,58 },{241,56 },{254,53 },{265,49 },{276,42 },{281,39 },{294,35 },{300,33 },{307,32 },{319,31 },{333,31 },{357,30 },{377,30 },{389,28 },{392,26 },{398,23 },{404,20 },{410,16 },{414,11 },{424,3 },{429,0}}},
    {57,
        {{529,1 },{532,15 },{532,19 },{523,30 },{518,35 },{510,49 },{508,55 },{504,66 },{499,77 },{490,92 },{469,112 },{464,116 },{453,124 },{440,134 },{431,138 },{423,143 },{412,147 },{399,154 },{383,160 },{374,164 },{365,167 },{342,171 },{325,178 },{305,186 },{299,190 },{281,203 },{275,208 },{270,213 },{259,228 },{253,241 },{245,259 },{243,275 },{243,286 },{243,300 },{244,309 },{249,318 },{253,334 },{253,345 },{253,358 },{246,371 },{235,384 },{228,394 },{225,397 },{219,403 },{206,411 },{197,415 },{190,417 },{182,419 },{168,423 },{159,425 },{147,429 },{131,434 },{115,440 },{103,447 },{93,452 },{73,464 },{57,477}}},
    {59,
        {{157,455 },{157,453 },{158,443 },{158,436 },{158,433 },{158,427 },{160,419 },{160,415 },{163,410 },{167,403 },{174,392 },{179,383 },{181,380 },{187,375 },{197,362 },{209,353 },{221,341 },{228,335 },{236,329 },{252,320 },{263,313 },{266,311 },{273,306 },{282,300 },{285,296 },{290,288 },{295,280 },{302,271 },{313,255 },{322,239 },{333,222 },{340,214 },{344,210 },{347,207 },{353,197 },{358,189 },{362,184 },{368,177 },{378,163 },{386,153 },{397,144 },{401,141 },{407,133 },{411,130 },{418,122 },{424,116 },{434,100 },{443,88 },{446,80 },{455,69 },{463,55 },{470,45 },{484,29 },{487,26 },{491,20 },{494,17 },{505,5 },{511,0 },{514,-4}}},
    {41,
        {{16,281 },{31,276 },{36,276 },{46,274 },{58,271 },{74,269 },{83,268 },{117,262 },{151,260 },{192,260 },{222,260 },{234,261 },{241,263 },{251,269 },{266,283 },{273,288 },{284,296 },{294,305 },{301,309 },{310,316 },{325,324 },{336,332 },{349,340 },{368,345 },{376,345 },{386,346 },{410,349 },{450,353 },{493,358 },{515,360 },{557,361 },{570,361 },{584,360 },{593,357 },{605,355 },{628,348 },{636,346 },{653,343 },{659,341 },{664,341 },{671,338}}},
    {63,
        {{2,333 },{7,332 },{14,331 },{29,329 },{36,327 },{44,325 },{61,320 },{83,318 },{91,318 },{114,314 },{131,311 },{150,308 },{168,307 },{181,306 },{191,302 },{212,297 },{221,295 },{230,294 },{239,292 },{244,291 },{251,288 },{259,286 },{274,285 },{277,284 },{290,283 },{305,283 },{313,283 },{322,283 },{326,283 },{337,287 },{349,291 },{354,293 },{365,294 },{374,295 },{381,295 },{389,297 },{400,300 },{450,307 },{467,308 },{480,308 },{503,308 },{525,307 },{531,307 },{538,305 },{559,304 },{564,304 },{568,304 },{574,304 },{584,304 },{594,304 },{599,304 },{608,306 },{612,308 },{619,311 },{623,312 },{627,312 },{636,312 },{645,315 },{650,315 },{655,315 },{664,317 },{667,318 },{674,323}}},
    {78,
        {{2,249 },{16,249 },{25,249 },{34,249 },{38,250 },{59,251 },{63,251 },{75,253 },{82,254 },{91,256 },{109,260 },{117,261 },{125,262 },{141,266 },{150,269 },{155,269 },{164,272 },{174,275 },{188,282 },{201,285 },{209,286 },{217,288 },{224,290 },{228,291 },{242,292 },{252,292 },{261,293 },{268,295 },{276,295 },{293,295 },{303,295 },{309,295 },{316,295 },{321,295 },{342,291 },{349,291 },{356,290 },{365,288 },{368,288 },{375,288 },{383,288 },{388,288 },{416,291 },{422,294 },{426,297 },{433,301 },{446,308 },{450,311 },{460,318 },{463,319 },{471,323 },{482,325 },{487,327 },{493,328 },{503,331 },{517,333 },{532,336 },{542,339 },{566,347 },{581,351 },{587,354 },{600,358 },{606,360 },{611,363 },{620,368 },{626,374 },{632,384 },{633,388 },{634,390 },{637,394 },{641,401 },{643,405 },{649,411 },{653,414 },{656,419 },{665,429 },{671,433 },{681,442}}},
};
const int MaxLength = 50;
struct Orbit {
    Point pointF;
    Point pointL;
    int attribute;
    bool key;
}orbit[MaxLength];//轨迹段集合TS
struct Cluster {
    Orbit centerOrbit;
    Orbit otherOrbit[MAX];
    int n;
};//轨迹簇分类


int orbitNum[MaxLength] = { 0 };//轨迹段条数

Trajectory trajectorySection[M];//划分后得到轨迹分段，这里是初始定义

//返回两个数中最小的数
double min_2N(double x, double y) {
    if (x<y)
        return x;
    else
        return y;
}
//s点到以a，b为端点的直线距离
double dis_2D(Point a, Point b, Point s) {
    double ab = sqrt(pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0));
    double as = sqrt(pow((a.x - s.x), 2.0) + pow((a.y - s.y), 2.0));
    double bs = sqrt(pow((s.x - b.x), 2.0) + pow((s.y - b.y), 2.0));
    
    if (as == 0)
        return 0;
    double cos_A = (pow(as, 2.0) + pow(ab, 2.0) - pow(bs, 2.0)) / (2 * ab*as);
    double sin_A = sqrt(1 - pow(cos_A, 2.0));
    return as*sin_A;
}
//s点到以a，b为端点的直线的垂点距离端点最小距离
double dis_2V(Point a, Point b, Point s) {
    double ab = sqrt(pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0));
    double as = sqrt(pow((a.x - s.x), 2.0) + pow((a.y - s.y), 2.0));
    double bs = sqrt(pow((s.x - b.x), 2.0) + pow((s.y - b.y), 2.0));
    
    if (as == 0)
        return 0;
    double cos_A = (pow(as, 2.0) + pow(ab, 2.0) - pow(bs, 2.0)) / (2 * ab*as);
    if (cos_A<0)
        return -as*cos_A;
    else {
        return as*cos_A ;
    }
}
//计算轨迹转角
double triangleAngle(Point point1, Point point2, Point point3) {
    double a, b, c, m0, m1;
    int flag;
    a = sqrt((point2.x - point1.x)*(point2.x - point1.x) + (point2.y - point1.y)*(point2.y - point1.y));
    b = sqrt((point3.x - point2.x)*(point3.x - point2.x) + (point3.y - point2.y)*(point3.y - point2.y));
    c = sqrt((point3.x - point1.x)*(point3.x - point1.x) + (point3.y - point1.y)*(point3.y - point1.y));
    m0 = acos((a*a + b*b - c*c) / (2 * a*b));
    flag = (point2.x - point1.x)*(point3.x - point2.x) + (point2.y - point1.y)*(point3.y - point2.y);
    if (flag >= 0) {
        m1 = pi - m0;
    }
    else {
        m1 = m0 - pi;
    }
    return m1;
}
//划分轨迹为轨迹分段(转角大于90度的点为特征点)
void trackSegment() {
    double flag;
    int m;
    for (int i = 0; i<M; i++) {
        //每个轨迹有0到n-3个转角
        m = 0;
        trajectorySection[i].point[m].x = trajectory[i].point[0].x;
        trajectorySection[i].point[m].y = trajectory[i].point[0].y;
        m = m + 1;
        for (int j = 0; j<trajectory[i].n - 2; j++) {
            flag = triangleAngle(trajectory[i].point[j], trajectory[i].point[j + 1], trajectory[i].point[j + 2]);
            if (fabs(flag) >= w0) {
                trajectorySection[i].point[m].x = trajectory[i].point[j + 1].x;
                trajectorySection[i].point[m].y = trajectory[i].point[j + 1].y;
                m++;
            }
        }
        trajectorySection[i].point[m].x = trajectory[i].point[trajectory[i].n - 1].x;
        trajectorySection[i].point[m].y = trajectory[i].point[trajectory[i].n - 1].y;
        trajectorySection[i].n = m + 1;
    }
}
//划分轨迹转换为轨迹段集合形式TS
int trackTurn() {
    int m = 0;
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<trajectorySection[i].n - 1; j++) {
            orbit[m].pointF.x = trajectorySection[i].point[j].x;
            orbit[m].pointF.y = trajectorySection[i].point[j].y;
            orbit[m].pointL.x = trajectorySection[i].point[j + 1].x;
            orbit[m].pointL.y = trajectorySection[i].point[j + 1].y;
            orbit[m].key = false;
            orbit[m].attribute = i;
            m++;
        }
        if (i >= 1)
            orbitNum[i] = m - orbitNum[i - 1];
        else
            orbitNum[i] = m;
    }
    return m;
}
//根据权重计算轨迹段的结构相似度
double DirDist(Orbit orbit1, Orbit orbit2) {
    double dirDist;
    double m, l, n, i, j, temp;
    m = sqrt(orbit1.pointF.x*orbit1.pointF.x + orbit1.pointF.y*orbit1.pointF.y);
    l = sqrt(orbit1.pointL.x*orbit1.pointL.x + orbit1.pointL.y*orbit1.pointL.y);
    n = acos((orbit1.pointF.x*orbit1.pointL.x + orbit1.pointF.y*orbit1.pointL.y) / (m*l));
    i = (orbit1.pointF.x - orbit1.pointL.x)*(orbit1.pointF.x - orbit1.pointL.x) + (orbit1.pointF.y - orbit1.pointL.y)*(orbit1.pointF.y - orbit1.pointL.y);
    j = (orbit2.pointF.x - orbit2.pointL.x)*(orbit2.pointF.x - orbit2.pointL.x) + (orbit2.pointF.y - orbit2.pointL.y)*(orbit2.pointF.y - orbit2.pointL.y);
    temp = sqrt(min_2N(i, j));
    if (0 <= n&&n <= (pi / 2)) {
        dirDist = temp*sin(n);
    }
    else if ((pi / 2) <= n&&n <= pi) {
        dirDist = temp;
    }
    else {
        dirDist = 0;
    }
    return dirDist;//角度
}
double AngleDist(Orbit orbit1, Orbit orbit2) {
    double angleDist1, angelDist2;
    double l1, l2, m1, m2;
    l1 = dis_2V(orbit1.pointF, orbit1.pointL, orbit2.pointF);
    l2 = dis_2V(orbit1.pointL, orbit1.pointF, orbit2.pointL);
    angleDist1 = min_2N(l1, l2);
    m1 = dis_2V(orbit2.pointF, orbit2.pointL, orbit1.pointF);
    m2 = dis_2V(orbit2.pointL, orbit2.pointF, orbit1.pointL);
    angelDist2 = min_2N(m1, m2);
    return min_2N(angleDist1, angelDist2);//转角,水平方向距离
}
double LocDist(Orbit orbit1, Orbit orbit2) {
    double locDist1, locDist2;
    double l1, l2, m1, m2;
    l1 = dis_2D(orbit1.pointF, orbit1.pointL, orbit2.pointF);
    l2 = dis_2D(orbit1.pointF, orbit1.pointL, orbit2.pointL);
    locDist1 = (l1*l1 + l2*l2) / (l1 + l2);
    m1 = dis_2D(orbit2.pointF, orbit2.pointL, orbit1.pointF);
    m2 = dis_2D(orbit2.pointF, orbit2.pointL, orbit1.pointL);
    locDist2 = (m1*m1 + m2*m2) / (m1 + m2);
    return min_2N(locDist1, locDist2);//位置,垂直距离
}

double SDIST(Orbit orbit1, Orbit orbit2) {
    double dirDist, angleDist, locDist;//SpeedDist权重为0,不考虑速度
    double SDIST;
    dirDist = DirDist(orbit1, orbit2);
    angleDist = AngleDist(orbit1, orbit2);
    locDist = LocDist(orbit1, orbit2);
    SDIST = WD*dirDist + WA*angleDist + WL*locDist;
    return SDIST;//SDIST越小，越相似
}

double SSIM(Orbit orbit1, Orbit orbit2) {
    return 1 - SDIST(orbit1, orbit2) / maxSDIDT;//归一化函数,SSIM越大，越相似
}

void Algorithm() {
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
    int lengthN = trackTurn();
    double maxTempSDIST = 0;
    double tempSD, tempSS;
    
    for (int i = 0; i<lengthN; i++) {
        for (int j = i + 1; j<lengthN; j++) {
            tempSD = SDIST(orbit[i], orbit[j]);
            if (tempSD>maxTempSDIST&&orbit[i].attribute != orbit[j].attribute)
                maxTempSDIST = tempSD;
        }
    }
    maxSDIDT = maxTempSDIST;//归一化要求最大值
    Cluster cluster[MAX];
    int m = 0, n, clusterNum = 0;
    bool flag = true;//开关
    for (int i = 0; i<lengthN; i++) {
        n = 0;
        if (orbit[i].key == false) {
            for (int j = i + 1; j<lengthN; j++) {
                tempSS = SSIM(orbit[i], orbit[j]);
                if (tempSS>e0&&orbit[i].attribute != orbit[j].attribute&&orbit[j].key == false)
                {
                    flag = false;
                    cluster[m].centerOrbit = orbit[i];
                    cluster[m].otherOrbit[n] = orbit[j];
                    orbit[i].key = true;
                    orbit[j].key = true;
                    n++;
                    cluster[m].n = n + 1;//一个聚类里面的轨迹分段数目
                }
                
            }
            if (flag) {
                cluster[m].centerOrbit = orbit[i];
                cluster[m].n = 1;
            }
            flag = true;
            m++;
        }
    }
    clusterNum = m;//聚类数
    
    //输出轨迹分段相似聚类的属性值即原来轨迹编号（未化简）
    for (int i = 0; i<m; i++) {
        cout << "center " << cluster[i].centerOrbit.attribute+1 << "(" << cluster[i].centerOrbit.pointF.x << ","
        << cluster[i].centerOrbit.pointF.y << ")->" << "(" << cluster[i].centerOrbit.pointL.x << ","
        << cluster[i].centerOrbit.pointL.y << ")" <<" other " ;
        for (int j = 0; j<cluster[i].n - 1; j++)
            cout << cluster[i].otherOrbit[j].attribute+1 << "(" << cluster[i].otherOrbit[j].pointF.x << ","
            << cluster[i].otherOrbit[j].pointF.y << ")->" << "(" << cluster[i].otherOrbit[j].pointL.x << ","
            << cluster[i].otherOrbit[j].pointL.y << ")" << " " ;
        cout << "\n";
    }//输出特征相似线段
    /*for (int i = 0; i<m; i++) {
     cout << "center " << cluster[i].centerOrbit.attribute << " other ";
     for (int j = 0; j<cluster[i].n - 1; j++)
     cout << cluster[i].otherOrbit[j].attribute << " ";
     cout << "\n";
     }*/
    
}

int main(int argc, const char * argv[]) {
    
    Algorithm();
    
    // insert code here...
    getchar();
    return 0;
}
