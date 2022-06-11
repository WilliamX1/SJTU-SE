#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

struct node
{
    int value;
    int time;
};
node A[10010];

int dp[10000010];

int main()
{
    int t, m; cin >> t >> m;
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &A[i].time, &A[i].value);

    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= m; i++)
        for (int j = A[i].time; j <= t; j++)
            for (int k = 1; k * A[i].time <= j; k++)
                dp[j] = max(dp[j], dp[j - k * A[i].time] + k * A[i].value);
    cout << dp[t];
    return 0;
}