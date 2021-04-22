#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int main()
{
    int n; scanf("%d", &n);
    double res = 1.0;

    n >>= 1;
    for (int i = 1; i < n; i++)
        res = res * (i + n - 1) / (i << 2);

    printf("%.4f", 1 - res);
    return 0;
}