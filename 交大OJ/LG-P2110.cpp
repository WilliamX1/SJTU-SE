#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
long long getsum(long long x)
{
    if (x <= 9) return x;

    long long res = x / 10 + 9;
    long long b = x; while(b >= 10) b /= 10;
    if (b > x % 10) --res;
    return res;

}
int main()
{
    long long L, R; scanf("%lld%lld", &L, &R);
    printf("%lld", getsum(R) - getsum(L - 1));
    return 0;
}