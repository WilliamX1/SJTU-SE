#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[250];
int dp[250][250];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);

    memset(dp, 0, sizeof(dp));

    int r, res = 0;
    for (int step = 0; step < n; step++)
        for (int l = 1; r = l + step, r <= n; l++)
        {
            if (!step) dp[l][r] = A[l];
            else
                for (int k = l; k <= r; k++)
                    if (dp[l][k] == dp[k + 1][r] && dp[l][k] && dp[k + 1][r])
                        dp[l][r] = max(dp[l][r], dp[l][k] + 1);
            res = max(dp[l][r], res);
        }
    /*for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << dp[i][j] << " ";
        cout << endl;
    }*/
    printf("%d", res);
    return 0;
}