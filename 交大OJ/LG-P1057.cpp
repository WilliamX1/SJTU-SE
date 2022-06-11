#include <bits/stdc++.h>

using namespace std;

long long dp[32][32];

int main()
{
    int n, m; cin >> n >> m;
    memset(dp, 0, sizeof(dp));

    dp[1][0] = 1;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (j == 1)
                dp[j][i] += dp[n][i - 1] + dp[j + 1][i - 1];
            else if (j == n)
                dp[j][i] += dp[j - 1][i - 1] + dp[1][i - 1];
            else dp[j][i] += dp[j - 1][i - 1] + dp[j + 1][i - 1];
    cout << dp[1][m];

    return 0;
}