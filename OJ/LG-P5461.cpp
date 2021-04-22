#include <bits/stdc++.h>

using namespace std;

bool A[1025][1025];

void Dfs(int x, int y, int len)
{
    if (len == 0) return;

    for (int i = x; i < x + len / 2; i++)
        for (int j = y; j < y + len / 2; j++)
            A[i][j] = 0;
    Dfs(x + len / 2, y + len / 2, len / 2);
    Dfs(x + len / 2, y, len / 2);
    Dfs(x, y + len / 2, len / 2);
}

int main()
{
    int n; cin >> n;
    n = pow(2.0, n);

    memset(A, 1, sizeof(A));

    Dfs(1, 1, n);
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << A[i][j] << " ";
        cout << endl;
    }

    return 0;
}