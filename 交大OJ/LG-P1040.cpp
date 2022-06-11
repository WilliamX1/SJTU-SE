#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
unsigned int A[32];
unsigned int dp[32][32];
unsigned int root[32][32];
void Dfs(int l, int r)
{
    if (l > r) return;
    printf("%d ", root[l][r]);

    Dfs(l, root[l][r] - 1);
    Dfs(root[l][r] + 1, r);
}
int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);

    for (int i = 1; i <= n; i++)
        dp[i][i] = A[i], dp[i][i - 1] = 1, root[i][i] = i;

    for (int i = n; i >= 1; i--)
        for (int j = i + 1; j <= n; j++)
            for (int k = i; k <= j; k++)
                if (dp[i][j] < dp[i][k - 1] * dp[k + 1][j] + A[k])
                {
                    dp[i][j] = dp[i][k - 1] * dp[k + 1][j] + A[k];
                    root[i][j] = k;
                }

    /*for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << dp[i][j] << " ";
        cout << "\n";
    }*/
    printf("%d\n", dp[1][n]);
    Dfs(1, n);
    return 0;
}