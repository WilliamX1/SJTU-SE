#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[128];
int dp[128][128];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);

    int m = n, res_M = 0, res_m = 0x7fffffff;
    while(m--)
    {
        //最小值
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (i == j) dp[i][j] = A[i];
                else dp[i][j] = 0x7fffffff;

        int r;
        for (int step = 1; step < n; step++)
            for (int l = 1; r = l + step, r <= n; l++)
                for (int k = r - 1; k >= l; k--)
                {
                    int sumL = 0, sumR = 0;
                    for (int i = l; i <= k; i++) sumL += A[i];
                    for (int i = k + 1; i <= r; i++) sumR += A[i];
                    if (k == l) sumL = 0;
                    if (k + 1 == r) sumR = 0;
                    dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + sumL + sumR);
                }
        res_m = min(res_m, dp[1][n]);

        //最大值
        memset(dp, 0, sizeof(dp));

        for (int step = 1; step < n; step++)
            for (int l = 1; r = l + step, r <= n; l++)
                for (int k = r - 1; k >= l; k--)
                {
                    int sumL = 0, sumR = 0;
                    for (int i = l; i <= k; i++) sumL += A[i];
                    for (int i = k + 1; i <= r; i++) sumR += A[i];
                    //if (k == l) sumL = 0;
                    dp[l][r] = max(dp[l][r], dp[l][k] + dp[k + 1][r] + sumL + sumR);
                }
        res_M = max(res_M, dp[1][n]);
        /*for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
                cout << dp[i][j] << " ";
            cout << endl;
        }*/

        A[0] = A[n];
        for (int i = n; i >= 1; i--)
            A[i] = A[i - 1];
    }
    printf("%d\n%d\n", res_m, res_M);

    return 0;
}
