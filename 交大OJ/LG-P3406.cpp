#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;


struct node
{
    long long Ai;
    long long Bi;
    long long Ci;
};
node Railway[100010];
long long int Road[100010];
long long dp[100010];

int main()
{
    int N, M; scanf("%d%d", &N, &M);
    for (int i = 1; i <= M; i++)
        scanf("%lld", &Road[i]);
    for (int i = 1; i <= N - 1; i++)
        scanf("%lld%lld%lld", &Railway[i].Ai, &Railway[i].Bi, &Railway[i].Ci);

    memset(dp, 0, sizeof(dp));
    int Start, End;
    for (int i = 1; i < M; i++)
    {
        Start = min(Road[i], Road[i + 1]);
        End = max(Road[i], Road[i + 1]);
        dp[Start]++;
        dp[End]--;
    }
    for (int i = 1; i < N; i++)
        dp[i] += dp[i - 1];

    long long res = 0;
    for (int i = 1; i < N; i++)
        res += min(dp[i] * Railway[i].Ai, dp[i] * Railway[i].Bi + Railway[i].Ci);
    cout << res;
    return 0;
}