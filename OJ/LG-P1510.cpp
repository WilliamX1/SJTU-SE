#include <bits/stdc++.h>

using namespace std;
int v[10001];
int c[10001];
int dp[10001];

int main()
{
    int V, N, C; cin >> V >> N >> C;
    for (int i = 1; i <= N; i++)
        cin >> v[i] >> c[i];

    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= N; i++)
        for (int j = C; j >= c[i]; j--)
            dp[j] = max(dp[j], dp[j - c[i]] + v[i]);

    int res = 0x7fffffff;
    for (int i = 0; i <= C; i++)
        if (dp[i] >= V)
            res = min(res, i);

    res = C - res;
    if (res >= 0) cout << res;
    else cout << "Impossible";

    return 0;
}