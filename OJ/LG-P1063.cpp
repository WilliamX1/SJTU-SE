#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[128];
int dp[128][128];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);

    int m = n, res = 0;
    while(m--)
    {
        A[0] = A[n]; A[n + 1] = A[1];

        memset(dp, 0, sizeof(dp));

        int r;
        for (int step = 1; step < n; step++)
            for (int l = 1; r = l + step, r <= n; l++)
                for (int k = r; k > l; k--)
                    dp[l][r] = max(dp[l][r], dp[l][k - 1] + dp[k][r] + A[l] * A[k] * A[r + 1]);
        res = max(res, dp[1][n]);

        A[0] = A[n];
        for (int i = n; i >= 1; i--)
            A[i] = A[i - 1];

        /*for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
                cout << dp[i][j] << " ";
            cout << endl;
        }*/
    }
    printf("%d", res);

    return 0;
}