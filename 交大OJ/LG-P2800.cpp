#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[1000010];
int dp[1000010][2];
int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    memset(dp, 0, sizeof(dp));

    dp[1][0] = 0, dp[1][1] = A[1];
    for (int i = 2; i <= n; i++)
    {
        dp[i][0] = min(dp[i - 1][1], dp[i - 2][1]);
        dp[i][1] = min(dp[i - 1][0], dp[i - 1][1]) + A[i];
    }
    printf("%d", min(dp[n][0], dp[n][1]));
    return 0;
}