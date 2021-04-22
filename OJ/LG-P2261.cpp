#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

int main()
{
    long long n, k; scanf("%lld%lld", &n, &k);

    long long ans = n * k;
    for (long long L = 1, R; L <= n; L = R + 1)
    {
        if (k / L != 0) R = min(k / (k / L), n);
        else R = n;
        ans -= (k / L) * (R - L + 1) * (L + R) / 2;
    }
    printf("%lld", ans);
    return 0;
}