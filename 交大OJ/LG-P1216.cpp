#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[1001][1001];
int dp[1001][1001];
int main()
{
    int r; cin >> r;
    for (int i = 1; i <= r; i++)
        for (int j = 1; j <= i; j++)
            scanf("%d", &A[i][j]);

    memset(dp, 0, sizeof(dp));
    dp[1][1] = A[1][1];
    for (int i = 2; i <= r; i++)
        for (int j = 1; j <= i; j++)
            dp[i][j] = max(dp[i - 1][j - 1], dp[i - 1][j]) + A[i][j];
    int res = 0;
    for (int j = 1; j <= r; j++)
        res = max(res, dp[r][j]);
    cout << res;


    return 0;
}