#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
long long int A[1024][1024];
int main()
{
    int N, M, C; scanf("%d%d%d", &N, &M, &C);

    memset(A, 0, sizeof(A));
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
        {
            scanf("%lld", &A[i][j]);
            A[i][j] += A[i - 1][j];
        }

    long long int res = 0, x_index, y_index;
    for (int i = 0; i <= N - C; i++)
    {
        int j = i + C;

        long long int tmp = 0;
        for (int k = 1; k <= C; k++)
            tmp += A[j][k] - A[i][k];

        if (tmp > res)
        {
            res = tmp;
            x_index = i + 1;
            y_index = 1;
        }
        for (int k = C + 1; k <= M; k++)
        {
            tmp += (A[j][k] - A[i][k]) - (A[j][k - C] - A[i][k - C]);
            if (tmp > res)
            {
                res = tmp;
                x_index = i + 1;
                y_index = k - C + 1;
            }
        }
    }
    cout << x_index << " " << y_index;

    return 0;
}