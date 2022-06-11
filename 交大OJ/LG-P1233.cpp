#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node
{
    int L;
    int R;
};
node A[5010];
bool nodeCompare(node a, node b)
{
    if (a.L == b.L)
        return a.R > b.R;
    return a.L > b.L;
}
int dp[5010];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &A[i].L, &A[i].R);
    sort(A + 1, A + n + 1, nodeCompare);
    //for (int i = 1; i <= n; i++)
        //cout << A[i].L << " " << A[i].R << endl;

    memset(dp, 0, sizeof(dp));

    int res = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = i - 1; j >= 0; j--)
            if (A[i].R > A[j].R)
                dp[i] = max(dp[i], dp[j] + 1);
        //cout << A[i].L << " " << A[i].R << " " << dp[i] << endl;
        res = max(res, dp[i]);
    }
    cout << res;
    return 0;
}