#include <bits/stdc++.h>

using namespace std;
int A[128];

int dp[110][10010];

int main()
{
    int N, M; cin >> N >> M;

    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= N; i++)
        cin >> A[i];

    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
        {
            if (j == A[i]) dp[i][j] = dp[i - 1][j] + 1;
            else if (j > A[i]) dp[i][j] = dp[i - 1][j] + dp[i - 1][j - A[i]];
            else if (j < A[i]) dp[i][j] = dp[i - 1][j];
        }
    cout << dp[N][M];

    return 0;
}