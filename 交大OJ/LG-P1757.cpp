#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int a[1001];
int b[1001];
int c[1001];
int dp[1001];

int main()
{
    int m, n; scanf("%d%d", &m, &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d%d", &a[i], &b[i], &c[i]);

    memset(dp, 0, sizeof(dp));
    for (int k = 1; k <= n; k++)
        for (int v = m; v >= 0; v--)
            for (int i = 1; i <= n; i++)
                if (c[i] == k && v >= a[i])
                    dp[v] = max(dp[v], dp[v - a[i]] + b[i]);

    cout << *max_element(dp, dp + m + 1);

    return 0;
}