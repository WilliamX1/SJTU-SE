#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[1024][1024];
int main()
{
    memset(A, 0, sizeof(A));

    int n, m; scanf("%d%d", &n, &m);

    int x1, y1, x2, y2;
    for (int i = 1; i <= m; i++)
    {
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        for (int j = x1; j <= x2; j++)
        {
            A[j][y1] += 1;
            A[j][y2 + 1] += -1;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            A[i][j] += A[i][j - 1];
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    return 0;
}