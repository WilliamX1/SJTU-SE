#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
long long mod;
long long res[3];
long long base[3][3];
void matrix_mul(long long A[][3], long long B[])
{
    long long B1 = (A[1][1] * B[1] + A[1][2] * B[2]) % mod;
    long long B2 = (A[2][1] * B[1] + A[2][2] * B[2]) % mod;
    B[1] = B1; B[2] = B2;
    return;
}
void matrix_self_mul(long long A[][3])
{
    long long A11 = (A[1][1] * A[1][1] + A[1][2] * A[2][1]) % mod;
    long long A12 = (A[1][1] * A[1][2] + A[1][2] * A[2][2]) % mod;
    long long A21 = (A[2][1] * A[1][1] + A[2][2] * A[2][1]) % mod;
    long long A22 = (A[2][1] * A[1][2] + A[2][2] * A[2][2]) % mod;
    A[1][1] = A11; A[1][2] = A12; A[2][1] = A21; A[2][2] = A22;
    return;
}
long long fastPower(long long n)
{
    if (n <= 2) return res[n];

    n -= 2;
    while(n > 0)
    {
        if (n & 1)
            matrix_mul(base, res);
        matrix_self_mul(base);
        n = n >> 1;
    }
    return res[1];
}
int main()
{
    long long p, q, a1, a2, n;
    scanf("%lld%lld%lld%lld%lld%lld", &p, &q, &a1, &a2, &n, &mod);
    res[1] = a2; res[2] = a1;
    base[1][1] = p; base[1][2] = q; base[2][1] = 1; base[2][2] = 0;
    printf("%lld", fastPower(n));

    return 0;
}