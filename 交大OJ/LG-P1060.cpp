#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int v[30];
int w[30];
int dp[32000];

int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)
    {
        scanf("%d%d", &v[i], &w[i]);
        w[i] *= v[i];
    }

    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= m; i++)
        for (int j = n; j >= v[i]; j--)
            dp[j] = max(dp[j], dp[j - v[i]] + w[i]);
    cout << *max_element(dp, dp + n);

    return 0;
}