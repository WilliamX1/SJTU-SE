#include <bits/stdc++.h>

using namespace std;

struct Nagato {
    int x, y;
    int rest;
    Nagato(int _x, int _y, int _r): x(_x), y(_y), rest(_r) {}
};

class Solution {
private:
    static constexpr int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

public:
    int shortestPath(vector<vector<int>>& grid, int k) {
        int m = grid.size(), n = grid[0].size();
        if (m == 1 && n == 1) {
            return 0;
        }

        k = min(k, m + n - 3);
        bool visited[m][n][k + 1];
        memset(visited, 0, sizeof(visited));
        queue<Nagato> q;
        q.emplace(0, 0, k);
        visited[0][0][k] = true;

        for (int step = 1; q.size() > 0; ++step) {
            int cnt = q.size();
            for (int _ = 0; _ < cnt; ++_) {
                Nagato cur = q.front();
                q.pop();
                for (int i = 0; i < 4; ++i) {
                    int nx = cur.x + dirs[i][0];
                    int ny = cur.y + dirs[i][1];
                    if (nx >= 0 && nx < m && ny >= 0 && ny < n) {
                        if (grid[nx][ny] == 0 && !visited[nx][ny][cur.rest]) {
                            if (nx == m - 1 && ny == n - 1) {
                                return step;
                            }
                            q.emplace(nx, ny, cur.rest);
                            visited[nx][ny][cur.rest] = true;
                        }
                        else if (grid[nx][ny] == 1 && cur.rest > 0 && !visited[nx][ny][cur.rest - 1]) {
                            q.emplace(nx, ny, cur.rest - 1);
                            visited[nx][ny][cur.rest - 1] = true;
                        }
                    }
                }
            }
        }
        return -1;
    }
};