#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

const int MAX = 10;

struct node {
    int x,y,fx,fy;
    int content;
    bool visited;
};
//迷宫存储：使用二维数组存储迷宫。二维数组中每个元素都是一个node，而每个node中包含：当前点的x，y值，前驱节点的x，y值，当前点的数据以及当前点是否已经被访问。记录前驱节点的x，y值方便最后的路径生成。
node laby[MAX][MAX];

void BFS(int sx, int sy,int ex, int ey) {
    
    queue<node> myQueue;
    
    node temp = laby[sx][sy];
    myQueue.push(temp);
    
    int delx[4] = {0,0,-1,1};
    int dely[4] = {1,-1,0,0};
    
    while (!myQueue.empty()) {
        node currentNode = myQueue.front();
        
        myQueue.pop();
        
        int m = currentNode.x;
        int n = currentNode.y;
        
        currentNode.visited = true;
        
        if (currentNode.x == ex && currentNode.y == ey) {
            break;
        }
        
        for(int i = 0; i < 4;i++) {
            int next_m = m + delx[i];
            int next_n = n + dely[i];
            if (next_m < 0 || next_m >= MAX || next_n < 0 || next_n >= MAX) continue;
            
            if (laby[next_m][next_n].content != 1 && laby[next_m][next_n].visited == false) {
                laby[next_m][next_n].fx = m;
                laby[next_m][next_n].fy = n;
                myQueue.push(laby[next_m][next_n]);
                laby[next_m][next_n].visited = true;
            }
        }
    }
    
    int backx = ex, backy = ey;
    int step = 0;
    stack<node> showStack;
    
    while (backx != sx || backy != sy) {
        
        showStack.push(laby[backx][backy]);
        
        int tempBackx = laby[backx][backy].fx;
        int tempBacky = laby[backx][backy].fy;
        
        backx = tempBackx;
        backy = tempBacky;
        
        step++;
    }
    
    cout<<"Path:"<<endl;
    while (!showStack.empty()) {
        node current = showStack.top();
        showStack.pop();
        cout<<'('<<current.x<<','<<current.y<<") ";
    }
    cout<<endl;
    
    cout<<"total steps:"<<step<<endl;
}


int main() {
    
    ifstream in;
    in.open("/Users/tangwei/Desktop/laby.txt",ios::in);
    
    if (!in) {
        cerr<<"file not existed!"<<endl;
        exit(1);
    }
    
    int sx,sy,ex,ey;
    
    int m = 0, n = 0;
    
    int map[MAX][MAX]={
        {5 ,1 ,0, 0 ,0 ,0 ,1 ,0 ,1, 0},
        {0 ,1, 0 ,1, 0 ,0 ,1 ,0, 1 ,0},
        {0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1, 0},
        {0 ,1, 1, 1 ,0 ,0 ,1 ,0 ,1 ,0},
        {0 ,0 ,0 ,1, 1 ,0 ,1, 0 ,0, 0},
        {0 ,1 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0},
        {0 ,1, 0 ,1 ,0, 1 ,1 ,0, 1, 0},
        {0 ,0, 0 ,0 ,0 ,0, 0, 0 ,1, 0},
        {0 ,1 ,1, 1 ,0 ,0 ,1, 0, 1 ,0},
        {0 ,0, 0 ,1 ,0 ,0 ,1, 0 ,1 ,8},
    };
    //迷宫描述：迷宫是一个二维数组。其中数字0表示能通过，数字1表示为障碍，数字5代表起点，数字8代表终点。程序首先从laby.txt文件中读取迷宫并存储在二维数组laby中。
    
    for(int i=0;i<MAX;i++)
    for(int j=0;j<MAX;j++){
        laby[i][j].content = map[i][j];
        laby[i][j].x = i;
        laby[i][j].y = j;
        laby[i][j].visited = false;
        if (map[i][j] == 5) {
            sx = i;
            sy = j;//开始
        }
        if (map[i][j] == 8) {
            ex = i;
            ey = j;//终点
        }
    }
    
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            cout<<laby[i][j].content<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    
    BFS(sx, sy, ex, ey);
    return 0;
}

