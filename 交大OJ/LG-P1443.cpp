#include <bits/stdc++.h>
#include <queue>
#include <stdio.h>
using namespace std;

int n, m;
struct node
{
    int x;
    int y;
    int steps;
    node(int the_x, int the_y, int t):x(the_x), y(the_y),steps(t){};
};
int A[500][500];

bool is(int x, int y) {return x >= 1 && x <= n && y >= 1 && y <= m;};

const int M = 1000000000 + 7;
int main()
{
    scanf("%d%d", &n, &m);
    int x, y, steps; scanf("%d%d", &x, &y);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            A[i][j] = M;

    queue<node> q;
    q.push(node(x, y, 1));

    while(!q.empty())
    {
        x = q.front().x;
        y = q.front().y;
        steps = q.front().steps;
        q.pop();
        if (A[x][y] <= steps) continue;

        A[x][y] = steps;

        if (is(x - 1, y - 2)) q.push(node(x - 1, y - 2, steps + 1));
        if (is(x - 2, y - 1)) q.push(node(x - 2, y - 1, steps + 1));
        if (is(x - 2, y + 1)) q.push(node(x - 2, y + 1, steps + 1));
        if (is(x - 1, y + 2)) q.push(node(x - 1, y + 2, steps + 1));
        if (is(x + 1, y + 2)) q.push(node(x + 1, y + 2, steps + 1));
        if (is(x + 2, y + 1)) q.push(node(x + 2, y + 1, steps + 1));
        if (is(x + 2, y - 1)) q.push(node(x + 2, y - 1, steps + 1));
        if (is(x + 1, y - 2)) q.push(node(x + 1, y - 2, steps + 1));
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (A[i][j] == M) A[i][j] = 0;
            printf("%-5d", A[i][j] - 1);
        }
        printf("\n");
    }
    return 0;
}