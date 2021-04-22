#include <iostream>
#include <memory.h>
using namespace std;

int A[25][25];
long long int dp[25][25];
long long int sum = 0;

void Dfs(int x, int y)
{
    if (A[x][y] == 2) {sum++; return;}

    if (A[x][y] == 1) return;

    Dfs(x + 1, y);
    Dfs(x, y + 1);
}

int main()
{
    memset(A, 0, sizeof(A));

    int n, m; cin >> n >> m;
    int x, y; cin >> x >> y;

    A[2][2] = 0;  //起点
    A[n + 2][m + 2] = 2; //终点
    A[x + 2][y + 2] = 1;
    A[x + 1][y] = A[x][y + 1] = A[x][y + 3] = A[x + 1][y + 4] = 1; //拦路点
    A[x + 3][y] = A[x + 4][y + 1] = A[x + 4][y + 3] = A[x + 3][y + 4] = 1;
    for (int i = 0; i <= n + 2; i++)
        A[i][1] = A[i][m + 3] = 1;
    for (int j = 0; j <= m + 2; j++)
        A[1][j] = A[n + 3][j] = 1;
    //Dfs(2, 2);

    memset(dp, 0, sizeof(dp));

    dp[2][2] = 1;

    for (int i = 2; i <= n + 2; i++)
        for (int j = 2; j <= m + 2; j++)
            if (i == 2 && j == 2) dp[i][j] = 1;
            else if (A[i][j] == 1) dp[i][j] = 0;
            else dp[i][j] = dp[i - 1][j] + dp[i][j - 1];

    cout << dp[n + 2][m + 2];
    //cout << sum;
    return 0;
}