#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[200010];
int dp[200010];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    dp[1] = A[1];
    for (int i = 2; i <= n; i++)
        dp[i] = dp[i - 1] > 0 ? dp[i - 1] + A[i] : A[i];
    cout << *max_element(dp + 1, dp + n + 1);

    return 0;
}