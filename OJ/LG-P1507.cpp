#include <bits/stdc++.h>

using namespace std;
int v[50];
int w[50];
int h[50];
int dp[400][400];

int main()
{
    int V, W; cin >> V >> W;
    int N; cin >> N;
    for (int i = 1; i <= N; i++)
        cin >> v[i] >> w[i] >> h[i];

    memset(dp, 0, sizeof(dp));

    int res = 0;
    for (int k = 1; k <= N; k++)
        for (int i = V; i > 0; i--)
            for (int j = W; j > 0; j--)
                if (v[k] <= i && w[k] <= j)
                {
                    dp[i][j] = max(dp[i][j], dp[i - v[k]][j - w[k]] + h[k]);
                    res = max(res, dp[i][j]);
                }
    cout << res;
    return 0;
}