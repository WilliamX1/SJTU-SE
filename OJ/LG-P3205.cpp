#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[1024];
const int mod = 19650827;
long long dp[1024][1024][2]; //0左1右

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);

    memset(dp, 0, sizeof(dp));
    int r;

    for (int step = 0; step < n; step++)
        for (int l = 1; r = l + step, r <= n; l++)
        {
            if (step == 0) dp[l][r][0] = dp[l][r][1] = 1;
            else if (step == 1)
                if (A[r] > A[l]) dp[l][r][0] = dp[l][r][1] = 1;
                else dp[l][r][0] = dp[l][r][1] = 0;
            else{
                if (A[r] > A[r - 1])
                    dp[l][r][1] += dp[l][r - 1][1];
                if (A[r] > A[l])
                    dp[l][r][1] += dp[l][r - 1][0];
                if (A[l] < A[l + 1])
                    dp[l][r][0] += dp[l + 1][r][0];
                if (A[l] < A[r])
                    dp[l][r][0] += dp[l + 1][r][1];
                dp[l][r][0] %= mod, dp[l][r][1] %= mod;
            }
        }
    printf("%lld", (dp[1][n][0] + dp[1][n][1]) % mod);
    return 0;
}