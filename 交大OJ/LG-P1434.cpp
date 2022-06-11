#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

int A[102][102];
int n1, n2;
int MAX = 1;
int T[110][110];

void Dfs(int x, int y, int len)
{
    if (len <= T[x][y]) return;

    MAX = max(MAX, len);
    T[x][y] = len;

    if (x - 1 >= 1 && A[x - 1][y] > A[x][y]) Dfs(x - 1, y, len + 1);
    if (x + 1 <= n1 && A[x + 1][y] > A[x][y]) Dfs(x + 1, y, len + 1);
    if (y - 1 >= 1 && A[x][y - 1] > A[x][y]) Dfs(x, y - 1, len + 1);
    if (y + 1 <= n2 && A[x][y + 1] > A[x][y]) Dfs(x, y + 1, len + 1);

    return;
}
int main()
{
    cin >> n1 >> n2;
    for (int i = 1; i <= n1; i++)
        for (int j = 1; j <= n2; j++)
            scanf("%d", &A[i][j]);

    memset(T, 0, sizeof(T));
    for (int i = 1; i <= n1; i++)
        for (int j = 1; j <= n2; j++)
            Dfs(i, j, 1);
    cout << MAX;

    return 0;
}