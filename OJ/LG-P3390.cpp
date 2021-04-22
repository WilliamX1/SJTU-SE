#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const long long Mod = 1000000000 + 7;
long long A[128][128];
long long res[128][128];
long long tmp[128][128];
long long n, k;

void matrix_mul(long long A[][128], long long B[][128])
{
    memset(tmp, 0, sizeof(tmp));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= n; k++)
                tmp[i][j] = (tmp[i][j] + A[i][k] * B[k][j]) % Mod;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            A[i][j] = tmp[i][j];
    return;
}
void fastPower(long long k)
{
    while(k > 0)
    {
        if (k & 1)
            matrix_mul(res, A);
        matrix_mul(A, A);
        k = k >> 1;
    }
    return;
}
void print()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            printf("%lld ", res[i][j]);
        printf("\n");
    }
}
int main()
{
    scanf("%lld%lld", &n, &k);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            scanf("%lld", &A[i][j]);
    for (int i = 1; i <= n; i++)
        res[i][i] = 1;

    fastPower(k);
    print();
    return 0;
}