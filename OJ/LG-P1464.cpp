#include <bits/stdc++.h>

using namespace std;
long long dp[21][21][21];
long long Dfs(int i, int j, int k)
{
    if (i <= 0 || j <= 0 || k <= 0) return 1;
    else if (i > 20 || j > 20 || k > 20) return Dfs(20, 20, 20);

    else if (dp[i][j][k]) return dp[i][j][k];
    else if (i < j && j < k)
        dp[i][j][k] = Dfs(i, j, k - 1) + Dfs(i, j - 1, k - 1) - Dfs(i, j - 1, k);
    else dp[i][j][k] = Dfs(i - 1, j, k) + Dfs(i - 1, j - 1, k) + Dfs(i - 1, j, k - 1) - Dfs(i - 1, j - 1, k - 1);
    return dp[i][j][k];
}
int main()
{
    memset(dp, 0, sizeof(dp));

    int a, b, c;
    while(cin >> a >> b >> c)
    {
        if (a == -1 && b == -1 && c == -1)
            break;
        else printf("w(%d, %d, %d) = %lld\n", a, b, c, Dfs(a, b, c));
    }
    return 0;
}