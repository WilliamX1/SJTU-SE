#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[120];
int dp[25010];

int main()
{
    int T; scanf("%d", &T);
    while(T--)
    {
        int n; scanf("%d", &n);

        memset(A, 0, sizeof(A));
        memset(dp, 0, sizeof(dp));
        for (int i = 1; i <= n; i++)
            scanf("%d", &A[i]);
        sort(A + 1, A + n + 1);

        int sum = 0;
        dp[0] = 1;
        for (int i = 1; i <= n; i++)
        {
            if (dp[A[i]] == 0)
                sum++;
            for (int j = A[i]; j <= A[n]; j++)
                dp[j] += dp[j - A[i]];
        }
        printf("%d\n", sum);
    }
    return 0;
}