#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const long long Mod = 1000000000 + 7;
long long res[4];
long long base[4][4];
long long tmp[4][4];
void matrix_mul(long long A[][4], long long B[])
{
    memset(tmp, 0, sizeof(tmp));
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= 3; j++)
            tmp[1][i] = (tmp[1][i] + A[i][j] * B[j]) % Mod;
    for (int i = 1; i <= 3; i++)
        B[i] = tmp[1][i];
    return;
}
void matrix_self_mul(long long A[][4], long long B[][4])
{
    memset(tmp, 0, sizeof(tmp));
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= 3; j++)
            for (int k = 1; k <= 3; k++)
                tmp[i][j] = (tmp[i][j] + A[i][k] * B[k][j]) % Mod;
    for (int i = 1; i <= 3; i++)
        for (int j = 1; j <= 3; j++)
            A[i][j] = tmp[i][j];
    return;
}
long long fastPowerMatrix(int n)
{
    if (n <= 3) return 1;
    memset(base, 0, sizeof(base));
    memset(res, 0, sizeof(res));
    res[0] = res[1] = res[2] = res[3] = 1;
    base[1][1] = base[1][3] = base[2][1] = base[3][2] = 1;

    n -= 3;
    while(n > 0)
    {
        if (n & 1)
            matrix_mul(base, res);
        matrix_self_mul(base, base);
        n = n >> 1;
    }
    return res[1];
}
int main()
{
    long long T; scanf("%lld", &T);
    while(T--)
    {
        int n;
        scanf("%d", &n);
        printf("%lld\n", fastPowerMatrix(n));
    }
    return 0;
}