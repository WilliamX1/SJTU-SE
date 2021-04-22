#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[3010];
int dp[3010][3010];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (i == j) dp[i][j] = 0;
            else if (j - i == 1)
            {
                if (A[j] == A[i]) dp[i][j] = 0;
                else dp[i][j] = 1;
            }
            else dp[i][j] = 0x7fffffff;

    int r;
    for (int step = 2; step < n; step++)
        for (int l = 1; r = l + step, r <= n; l++)
        {
            if (A[l] == A[r]) dp[l][r] = min(dp[l][r], dp[l + 1][r - 1]);
            else dp[l][r] = min(dp[l][r], dp[l + 1][r - 1] + 1);

            dp[l][r] = min(dp[l][r], dp[l + 1][r] + 1);
            dp[l][r] = min(dp[l][r], dp[l][r - 1] + 1);
        }
    printf("%d", dp[1][n]);
    return 0;
}