#include <iostream>
#include <memory.h>
using namespace std;

int A[50][100];
int main()
{
    memset(A, 0, sizeof(A));

    int n; cin >> n;
    A[1][1] = 1;

    int x1, x2, yushu = 0;
    for (int i = 2; i <= n; i++)
    {
        x1 = i % 10;
        x2 = i / 10;
        for (int j = 1; j < 100; j++)
        {
            A[i][j] = x1 * A[i - 1][j] + x2 * A[i - 1][j - 1] + yushu;
            yushu = A[i][j] / 10;
            A[i][j] %= 10;
        }
    }

    int res[100];
    memset(res, 0, sizeof(res));

    yushu = 0;
    for (int i = 1; i < 100; i++)
    {
        for (int j = 1; j <= n; j++)
            res[i] += A[j][i];
        res[i] += yushu;
        yushu = res[i] / 10;
        res[i] %= 10;
    }

    int xb = 99;
    for (; xb >= 1 && res[xb] == 0; xb--);
    for (; xb >= 1; xb--)
        cout << res[xb];
    return 0;
}