#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node{
    int start, last_minute;
    node():start(0), last_minute(0){};
    node(int s, int l):start(s), last_minute(l){};
};
node A[10010];
bool nodeCompare(node a, node b){
    return a.start < b.start;
}
int dp[10010];

int main()
{
    int n, k; scanf("%d%d", &n, &k);
    for (int i = 1; i <= k; i++)
        scanf("%d%d", &A[i].start, &A[i].last_minute);
    sort(A + 1, A + k + 1, nodeCompare);

    memset(dp, 0, sizeof(dp));
    int cur_work_index = k;

    for (int i = n; i >= 1; i--)
    {
        bool flag = true;
        while (A[cur_work_index].start == i)
        {
            dp[i] = max(dp[i], dp[A[cur_work_index].start + A[cur_work_index].last_minute]);
            cur_work_index--;
            flag = false;
        }
        if (flag) dp[i] = dp[i + 1] + 1;
    }
    cout << dp[1];
    return 0;
}