#include <iostream>
#include <set>
#include <vector>

using namespace std;

/*
 * 输入一个无向图的所有边,以vector<vector<int>>表示,而边用长度为2的vector<int>表示,包含其两端顶点的编号.
 * 该图有n个节点和n个边,顶点编号从1到n. 它由一棵树以及一条附加的边组成.
 * 该附加边的两端点是点1到点n中的点,且两端点不相同,也不是树中已经存在的边.
 * 返回一条可以从图中删掉的边,用长度为2的vector<int>表示,使得该图成为一棵树.如果有多个边可以做为被删掉的边，那么返回输入中的最后一条可以删除的边
 *
 * 请完成findRedundantConnection函数的编写, 提供代码与运行截图,输出应当是3 4
 * 分析你的实现的时间复杂度,和代码/运行截图放在一起提交
 * 示例:
 */

void print_vec(const vector<int> &p) {
    for (auto &i:p)
        cout << i << ' ';
    cout << endl;
}

vector<int> findRedundantConnection(vector<vector<int>> &edges) {
    // your implementation
    /* 用邻接表存放边 */
    const int n = edges.size();
    vector<vector<int>> table(n + 1);
    for (int i = 0; i < n; i++)
    {
        table[edges[i][0]].push_back(edges[i][1]);
        table[edges[i][1]].push_back(edges[i][0]);
    }
    /* Prim算法判断，用两个集合表示已经加入的点和还未加入的点 */
    set<int> S, U;
    vector<int> ans;
    typedef struct edge
    {
        /* data */
        int x;
        int y;
        edge(int xx, int yy) : x(xx), y(yy) {};
    } edge_t;
    vector<edge_t> K; // 存放待选择的边
    for (int i = 1; i <= n; i++) U.insert(i);
    /* 随便选取一个点作为起点 */
    U.erase(1); S.insert(1);
    auto iter = table[1].begin();
    while (iter != table[1].end()) { // 将这个点相邻的边全部加入待选择边数组K中
        K.push_back(edge(1, *iter));
        ++iter;
    };
    while (!S.empty()) {
        edge cur = K.back();
        K.pop_back();

        if (S.find(cur.x) != S.end() && S.find(cur.y) != S.end()) {
            /* 寻找结果数组，防止输出反了 */
            for (int i = 0; i < n; i++) {
                if (edges[i][0] == cur.x && edges[i][1] == cur.y) {
                    ans.push_back(cur.x);
                    ans.push_back(cur.y);
                    break;
                } else if (edges[i][1] == cur.x && edges[i][0] == cur.y) {
                    ans.push_back(cur.y);
                    ans.push_back(cur.x);
                    break;
                }
            };
            return ans;
        } else if (S.find(cur.x) == S.end()) {
            U.erase(cur.x); S.insert(cur.x);
            iter = table[cur.x].begin();
            while (iter != table[cur.x].end()) {
                K.push_back(edge(cur.x, *iter));
                ++iter;
            };
        } else if (S.find(cur.y) == S.end()) {
            U.erase(cur.y); S.insert(cur.y);
            iter = table[cur.y].begin();
            while (iter != table[cur.y].end()) {
                K.push_back(edge(cur.y, *iter));
                ++iter;
            };
        } else {
            printf("Error: this is not possible!");
        };
    };
}

int main() {
    /*
     * 1 - 2 - 3
     *     |   |
     *     5 - 4
     */
    /*
     * 1 - 2
     * |   |
     * --- 3
     */
    vector<vector<int>> edges = {{1, 2},
                                 {3, 4},
                                 {2, 3},
                                 {2, 5},
                                 {4, 5}};
    vector<int> edge = findRedundantConnection(edges);
    print_vec(edge);
} // 应当输出3 4