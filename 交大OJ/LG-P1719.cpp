#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[121][121];
int dp[121];  //dp[i][j]表示第i列第j行时最大值

int main()
{
    int N; scanf("%d", &N);
    memset(A, 0, sizeof(A));
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
        {
            scanf("%d", &A[i][j]);
            A[i][j] += A[i - 1][j];
        }

    int res = 0;
    for (int i = 0; i <= N; i++)
        for (int j = i + 1; j <= N; j++)
        {
            memset(dp, 0, sizeof(dp));
            for (int k = 1; k <= N; k++)
                dp[k] = A[j][k] - A[i][k];
            for (int k = 1; k <= N; k++)
                if (dp[k - 1] > 0) dp[k] += dp[k - 1];
            res = max(res, *max_element(dp + 1, dp + N + 1));
        }
    cout << res;

    return 0;
}