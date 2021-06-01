/*  Graph Lab
    id: 519021910861
    name: 徐惠东
    date: 2021-5-17
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

using namespace std;

const int64_t INF = 1000000000000000;
int64_t n;

struct node {
    int64_t v2;
    int64_t weight;
    node(int64_t _v2, int64_t _weight)
    : v2(_v2), weight(_weight) {};
};
bool operator < (node a, node b) {
    if (a.weight > b.weight) return 1;
    else return 0;
};

/* 节点的度越大 => 节点的度中心性越高*/
void getDegreeCentralityMax(const vector<vector<node*>> adjacency_list, int64_t& v, int64_t& degree) {
    // int64_t n = adjacency_list.size();
    v = -1; // 设置下标初始值
    degree = 0; // 设置度初始值
    int64_t tmp = 0;
    for (int64_t i = 0; i < n; i++)
    {
        tmp = adjacency_list[i].size();
        if (tmp > degree) {
            v = i;
            degree = tmp;
        }
    }
    return;
}
/* 节点的最短路径和越小 => 节点的亲近中心性越高 */
void getClosenessCentrality(const vector<vector<int64_t>> shortest_path, int64_t& v, double& degree) {
    v = -1, degree = 0.0;

    // int64_t n = shortest_path.size();
    int64_t tmp_int64_t = 0;
    double tmp_double = 0.0;
    for (int64_t i = 0; i < n; i++)
    {
        // tmp = 0;
        tmp_int64_t = accumulate(shortest_path[i].begin(), shortest_path[i].end(), 0);
        // for (int64_t j = 0; j < n; j++)
        //     tmp += shortest_path[i][j];
        tmp_double = double(n - 1) / double(tmp_int64_t);
        if (tmp_double > degree) {
            v = i;
            degree = tmp_double;
        };
    };
    return;
}
/* 节点的出现在其他点对之间的最短路径的次数越多 => 节点的中介中心性越高 */
void getBetweennessCentrality(const vector<vector<int64_t>> shortest_path, int64_t& v, int64_t& degree, vector<int64_t> BC) {
    v = -1, degree = 0;
    // vector<int64_t> BC(n, 0);
    // BC.assign(n, 0);
    // for (int64_t i = 0; i < n; i++)
    //     for (int64_t j = i + 1; j < n; j++)
    //         for (int64_t k = 0; k < n; k++)
    //             if (k == i || k == j) continue;
    //             else if (shortest_path[i][j] == shortest_path[i][k] + shortest_path[k][j]) BC[k]++;
    for (int64_t i = 0; i < n; i++)
    {
        BC[i] >>= 1;
        if (BC[i] > degree) {
            degree = BC[i];
            v = i;
        }
    }
        
    return;
}
/* 获得两个点之间的最短路径 */
void getShortestPath(const vector<vector<node*>> adjacency_list, vector<vector<int64_t>>& shortest_path, vector<int64_t>& BC)
{
    /* 使用迪杰特斯拉算法计算每个点之间的最短路径 */
    // int64_t n = adjacency_list.size();
    // set<int64_t> S, U;
    // for (int64_t i = 0; i < n; i++) U.insert(i);
    // for (int64_t start = 0; start < n; start++)
    // {
    //     /* S用来记录已求出最短路径的顶点，初始时只包含起点*/
    //     /* U用来记录待求的顶点，包含除顶点之外的所有点 */
    //     U.erase(start);
    //     S.insert(start);  
    //     /*  从U中选出距离最短的顶点k，将k加入到S中，并从U中移除k
    //         更新和k相关的顶点与起点的距离
    //     */
    //     while (!U.empty()) {
    //         /* 离该点最近的点 */
    //         auto iter = U.begin();
    //         int64_t k = -1, ans = INF;
    //         while (iter != U.end()) {
    //             if (shortest_path[start][(*iter)] < ans) {
    //                 k = (*iter);
    //                 ans = shortest_path[start][(*iter)];
    //             }
    //             iter++;
    //         };
    //         /* 将顶点k加入S中，并从U中删除 */
    //         S.insert(k); U.erase(k);
    //         /* 更新与k相关的顶点的路径 */
    //         auto cursor = adjacency_list[k].begin();
    //         while (cursor != adjacency_list[k].end()) {
    //             shortest_path[start][(*cursor)->v2] = min(shortest_path[start][(*cursor)->v2],
    //             shortest_path[start][k] + shortest_path[k][(*cursor)->v2]);
    //             cursor++;
    //         };
    //     }
    //     /* 交换这两个集合 */
    //     U = S;
    //     S.clear();
    // }
    /* 使用迪杰特斯拉算法（堆优化）计算每个点之间的最短路径 */
    priority_queue<node> U;
    /* 节点中介中心性 */
    BC.assign(n, 0);
    vector<int64_t> father(n, -1); // 每个节点的父节点（谁让它变短了谁就是父节点）

    for (int64_t start = 0; start < n; start++)
    {
        /* 将与起点相邻的边加入队列 */
        auto iter = adjacency_list[start].begin();
        while (iter != adjacency_list[start].end()) {
            U.push(node((*iter)->v2, (*iter)->weight));
            iter++;
        }

        father.assign(n, -1);
        while (!U.empty()) {
            node cursor = U.top();
            /* 如果顶点路径长度变化则无意义 */
            if (cursor.weight > shortest_path[start][cursor.v2]) {
                U.pop();
                continue;
            }
            /* 将顶点从U中删除 */
            U.pop();
            /* 更新与顶点相关的路径 */
            iter = adjacency_list[cursor.v2].begin();
            while (iter != adjacency_list[cursor.v2].end()) {
                if (shortest_path[start][cursor.v2] + shortest_path[cursor.v2][(*iter)->v2] < shortest_path[start][(*iter)->v2]) {
                    shortest_path[start][(*iter)->v2] = shortest_path[start][cursor.v2] + shortest_path[cursor.v2][(*iter)->v2];
                    U.push(node((*iter)->v2, shortest_path[start][(*iter)->v2]));
                    father[(*iter)->v2] = cursor.v2;
                };
                iter++;
            };
        };

        for (int i = 0; i < n; i++)
        {
            int pre = father[i];
            while (pre != -1) {
                BC[pre]++;
                pre = father[pre];
            };
        };
    }
    /* 使用弗洛伊德算法计算每个点之间的最短路径 */
    // for (int64_t k = 0; k < n; k++)
    //     for (int64_t i = 0; i < n; i++)
    //         for (int64_t j = 0; j < n; j++)
    //             shortest_path[i][j] = min(shortest_path[i][j], shortest_path[i][k] + shortest_path[k][j]);
 
    /* 打印最短路径 */
    // cout << "---------打印最短路径---------" << endl;
    // for (int64_t i = 0; i < n; i++)
    // {
    //     for (int64_t j = 0; j < n; j++)
    //         cout << shortest_path[i][j] << ' ';
    //     cout << endl;
    // }
    return;
}
int main(int argc, char* argv[])
{   
    // 以读模式打开文件
    ifstream infile; 
    infile.open(argv[1]);
    /* 节点总数 */
    infile >> n;   
    /* 用集合存放邻接表 adjacency_list[i]代表和*/
    vector<vector<node*>> adjacency_list(n);
    /* 用数组存放两个点之间最短路径长度 */
    vector<vector<int64_t>> shortest_path(n, vector<int64_t>(n, INF));
    for (int64_t i = 0; i < n; i++) shortest_path[i][i] = 0; // 自身到自身的距离为0
    /* 塞入每个结点 */
    int64_t v1, v2, weight;
    char* line = new char[100];

    while (infile >> line) {
        // cout << line << endl;
        char *p = strtok(line, ",");
        v1 = atoi(p);
        p = strtok(NULL, ",");
        v2 = atoi(p);
        p = strtok(NULL, ",");
        weight = atoi(p);
        adjacency_list[v1].push_back(new node(v2, weight));
        adjacency_list[v2].push_back(new node(v1, weight));
        /* 先初始化最短路径数组，有直接连边的则记录该数值，无直接连边的则设置为无穷大 */
        shortest_path[v1][v2] = shortest_path[v2][v1] = weight;
    }
    // 关闭打开的文件
    infile.close();

    /* 获取单源最短路径 */
    vector<int64_t> BC;
    getShortestPath(adjacency_list, shortest_path, BC);

    /* 获取度中心性最大的节点 */
    int64_t DC_v = -1, DC_d = 0;
    getDegreeCentralityMax(adjacency_list, DC_v, DC_d);
    cout << DC_v << ' ' << DC_d << endl;
    // print64_tf("%d %d\n", DC_v, DC_d);
    // print64_tf("---------度中心性--------- DC_v = %d, DC_d = %d \n", DC_v, DC_d);
    
    /* 获取亲近中心性最大的节点 */
    int64_t CC_v = -1; double CC_d = 0.0;
    getClosenessCentrality(shortest_path, CC_v, CC_d);
    cout << CC_v << ' ' << CC_d << endl;
    // print64_tf("%d %.6f\n", CC_v, CC_d);
    // print64_tf("---------亲近中心性-------- CC_v = %d, CC_d = %f \n", CC_v, CC_d);
    
    /* 获取中介中心性最大的节点 */
    int64_t BC_v = -1, BC_d = 0;
    getBetweennessCentrality(shortest_path, BC_v, BC_d, BC);
    cout << BC_v << ' ' << BC_d << endl;
    // print64_tf("%d %d\n", BC_v, BC_d);
    // print64_tf("---------中介中心性-------- BC_v = %d, BC_d = %d \n", BC_v, BC_d);
    
    return 0;
}