#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node
{
    int value;
    int time;
};
node A[110];

int dp[1010];

int main()
{
    int T, M; scanf("%d%d", &T, &M);
    for (int i = 1; i <= M; i++)
        scanf("%d%d", &A[i].time, &A[i].value);

    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= M; i++)
        for (int j = T; j >= A[i].time; j--)
            dp[j] = max(dp[j], dp[j - A[i].time] + A[i].value);


    cout << dp[T];
    return 0;
}