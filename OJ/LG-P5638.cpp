#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

long long A[1000010];

int main()
{
    int n, k; scanf("%d%d", &n, &k);
    long long tot = 0;
    for (int i = 1; i < n; i++)
    {
        scanf("%lld", &A[i]);
        tot += A[i];
    }
    if (k == 0)
        printf("%lld", tot);
    else if (k >= n - 1)
        printf(0);
    else
    {
        long long int res = 0;
        long long int tmp = 0;
        for (int i = 1; i <= k; i++)
            tmp += A[i];
        res = max(res, tmp);
        for (int i = k + 1; i <= n; i++)
        {
            tmp += A[i] - A[i - k];
            res = max(res, tmp);
        }
        printf("%lld", tot - res);
    }
    return 0;
}