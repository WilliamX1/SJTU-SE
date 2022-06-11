#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

struct node
{
    int lose;
    int win;
    int use;
};
node A[1010];
int dp[1010];
int main()
{
    int n, x; cin >> n >> x;
    for (int i = 1; i <= n; i++)
        cin >> A[i].lose >> A[i].win >> A[i].use;

    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= n; i++)
    {
        for (int j = x; j >= A[i].use; j--)
            dp[j] = max(dp[j] + A[i].lose, dp[j - A[i].use] + A[i].win);
        for (int j = A[i].use - 1; j >= 0; j--)
            dp[j] += A[i].lose;
    }
    printf("%lld", 5ll * dp[x]);
    return 0;
}