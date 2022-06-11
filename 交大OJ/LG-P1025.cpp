#include <bits/stdc++.h>

using namespace std;
int dp[256][8];
int main()
{
    int n, k; cin >> n >> k;
    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= min(i, k); j++)
        {
            if (i < j) dp[i][j] = 0;
            else if (i == j) dp[i][j] = 1;
            else
                dp[i][j] = dp[i - 1][j - 1] + dp[i - j][j];
        }
    cout << dp[n][k];

    return 0;
}