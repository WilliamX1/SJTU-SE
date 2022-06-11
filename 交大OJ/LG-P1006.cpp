#include <bits/stdc++.h>
#include <stdio.h>
#include <memory.h>
#include <cmath>
using namespace std;

int A[60][60];
int dp[60][60][60][60];

int main()
{
    int n, m; scanf("%d%d", &n, &m);

    memset(A, 0, sizeof(A));
    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%d", &A[i][j]);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int k = 1; k <= n; k++)
                for (int s = 1; s <= m; s++)
                {
                    if (i == 1 && j == 1 && k == 1 && s == 1)
                        dp[i][j][k][s] = 0;
                    else
                        if (i == k && j == s && (i != n || j != m)) dp[i][j][k][s] -= 1e9;
                        else dp[i][j][k][s] = A[i][j] + A[k][s] + max(max(dp[i - 1][j][k - 1][s], dp[i - 1][j][k][s - 1]), max(dp[i][j - 1][k - 1][s], dp[i][j - 1][k][s - 1]));
                }
    cout << dp[n][m][n][m];
    return 0;
}