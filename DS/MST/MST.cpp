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
    /* Prim算法判断，用两个集合表示已经加入的点和还未加入的点 */
    int n = edges.size();
    set<int> S, U, K; // S用于记录已经加入的点，U用于记录还未加入的点
    for (int i = 1; i <= n; i++) U.insert(i);
    S.insert(edges[0][0]); U.erase(edges[0][0]); // 选定初始点
    S.insert(edges[0][1]); U.erase(edges[0][1]);
    K.insert(0); // K用来记录走过的边编号
    vector<int> ans; // 返回的结果数组
    int i = 0;
    while (!U.empty()) {
        i = (i + 1) % n;
        if (K.find(i) != K.end()) continue;
        else if (S.find(edges[i][0]) != S.end() && S.find(edges[i][1]) != S.end()) {
            ans.push_back(edges[i][0]); 
            ans.push_back(edges[i][1]);
            break;
        }
        else if (U.find(edges[i][0]) != U.end() && U.find(edges[i][1]) != U.end()) continue;
        else if (S.find(edges[i][0]) != S.end() && U.find(edges[i][1]) != U.end()) {
            S.insert(edges[i][1]); U.erase(edges[i][1]); K.insert(i);
        } else if (S.find(edges[i][1]) != S.end() && U.find(edges[i][0]) != U.end()) {
            S.insert(edges[i][0]); U.erase(edges[i][0]); K.insert(i);
        };
    };
    if (ans.empty()) {
        for (i = 0; i < n; i++)
            if (K.find(i) == K.end()) {
                ans.push_back(edges[i][0]);
                ans.push_back(edges[i][1]);
                break;
            }
    };
    return ans;
}

int main() {
    /*
     * 1 - 2 - 3
     *     |   |
     *     5 - 4
     */
    vector<vector<int>> edges = {{1, 2},
                                 {2, 3},
                                 {2, 5},
                                 {4, 5},
                                 {3, 4}};
    vector<int> edge = findRedundantConnection(edges);
    print_vec(edge);
} // 应当输出3 4