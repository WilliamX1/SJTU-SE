#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int dp[2001][2001];  //表示第N步用第M个小组
int T[2001][2001];

int main()
{
    int N, M; scanf("%d%d", &N, &M);
    for (int j = 1; j <= M; j++)
        for (int i = 1; i <= N; i++)
            scanf("%d", &T[i][j]);

    for (int j = 0; j <= M; j++) dp[0][j] = 0;

    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
            if (j == 1)
                dp[i][j] = min(dp[i - 1][j], dp[i - 1][M]) + T[i][j];
            else
                dp[i][j] = min(dp[i - 1][j], dp[i - 1][j - 1]) + T[i][j];

    printf("%d", *min_element(dp[N] + 1, dp[N] + M));

    return 0;
}