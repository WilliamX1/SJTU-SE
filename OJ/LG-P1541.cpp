#include <bits/stdc++.h>
using namespace std;

int A[45][45][45][45];

int score[400];
int B[5];

int main()
{
    int N, M; cin >> N >> M;

    for (int i = 0; i < N; i++)
        cin >> score[i];

    memset(B, 0, sizeof(B));
    int tmp;
    for (int i = 0; i < M; i++)
    {
        cin >> tmp;
        B[tmp]++;
    }

    memset(A, 0, sizeof(A));
    A[0][0][0][0] = score[0];

    for (int i = 0; i <= B[1]; i++)
        for (int j = 0; j <= B[2]; j++)
            for (int k = 0; k <= B[3]; k++)
                for (int s = 0; s <= B[4]; s++)
                {
                    int r = i + 2 * j + 3 * k + 4 * s;
                    if (i > 0) A[i][j][k][s] = max(A[i][j][k][s], A[i - 1][j][k][s] + score[r]);
                    if (j > 0) A[i][j][k][s] = max(A[i][j][k][s], A[i][j - 1][k][s] + score[r]);
                    if (k > 0) A[i][j][k][s] = max(A[i][j][k][s], A[i][j][k - 1][s] + score[r]);
                    if (s > 0) A[i][j][k][s] = max(A[i][j][k][s], A[i][j][k][s - 1] + score[r]);
                }
    cout << A[B[1]][B[2]][B[3]][B[4]];
    return 0;
}