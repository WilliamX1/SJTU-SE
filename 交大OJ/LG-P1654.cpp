#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

double x1[100010];
double x2[100010];
double ans[100010];
double p[100010];

int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> p[i];

    x1[0] = x2[0] = ans[0] = 0;
    for (int i = 1; i <= n; i++)
    {
        x1[i] = (x1[i - 1] + 1) * p[i];
        x2[i] = (x2[i - 1] + 2 * x1[i - 1] + 1) * p[i];
        ans[i] = ans[i - 1] + (3 * x1[i - 1] + 3 * x2[i - 1] + 1) * p[i];
    }
    printf("%.1f", ans[n]);
    return 0;
}