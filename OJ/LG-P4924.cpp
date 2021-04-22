#include <bits/stdc++.h>
using namespace std;

int A[510][510];

int main()
{
    int n, m; cin >> n >> m;
    memset(A, 0, sizeof(A));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            A[i][j] = (i - 1) * n + j;

    int x, y, r, z;
    int LU_x, LU_y, RU_x, RU_y, RD_x, RD_y, LD_x, LD_y;
    int tmp;
    for (int i = 1; i <= m; i++)
    {
        cin >> x >> y >> r >> z;
        for (int j = 1; j <= r; j++)
            for (int k = 0; k < 2 * j; k++)
            {
                LU_x = x - j; LU_y = y - j;
                RU_x = x - j; RU_y = y + j;
                RD_x = x + j; RD_y = y + j;
                LD_x = x + j; LD_y = y - j;
                if (z)
                {
                    tmp = A[LU_x + k][LU_y];
                    A[LU_x + k][LU_y] = A[RU_x][RU_y - k];
                    A[RU_x][RU_y - k] = A[RD_x - k][RD_y];
                    A[RD_x - k][RD_y] = A[LD_x][LD_y + k];
                    A[LD_x][LD_y + k] = tmp;
                }
                else
                {
                    tmp = A[LU_x][LU_y + k];
                    A[LU_x][LU_y + k] = A[LD_x - k][LD_y];
                    A[LD_x - k][LD_y] = A[RD_x][RD_y - k];
                    A[RD_x][RD_y - k] = A[RU_x + k][RU_y];
                    A[RU_x + k][RU_y] = tmp;
                }
            }
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << A[i][j] << " ";
        cout << endl;
    };

    return 0;
}