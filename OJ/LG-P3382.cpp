#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int N;
double A[15];

double f(double x)
{
    double res = 0;
    double ans = 1;
    for (int i = 0; i <= N; i++)
    {
        res += A[i] * ans;
        ans *= x;
    }
    return res;
}

int main()
{
    scanf("%d", &N);
    double l, r; scanf("%lf%lf", &l, &r);
    for (int i = N; i >= 0; i--)
        scanf("%lf", &A[i]);

    double lmid = (2 * l + r) / 3, rmid = (2 * r + l) / 3;
    while(fabs(rmid - lmid) > 1e-6)
    {
        if (f(lmid) <= f(rmid))
            l = lmid;
        else r = rmid;

        lmid = (2 * l + r) / 3;
        rmid = (2 * r + l) / 3;
    }
    printf("%.5lf", lmid);
    return 0;
}