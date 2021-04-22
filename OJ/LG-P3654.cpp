#include <bits/stdc++.h>

using namespace std;

char A[110][110];
char B[110][110];

int main()
{
    int R, C, K; cin >> R >> C >> K;

    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            { cin >> A[i][j]; B[i][j] = A[i][j];};

    for (int i = 1; i <= R; i++)
        A[i][C + 1] = B[i][C + 1] = '#';
    for (int j = 1; j <= C; j++)
        A[R + 1][j] = B[R + 1][j] = '#';


    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C + 1; j++)
            if (A[i][j] == '#')
                for (int k = j - 1; k > max(j - K, 0); k--)
                    A[i][k] = '#';

    for (int j = 1; j <= C; j++)
        for (int i = 1; i <= R + 1; i++)
            if (B[i][j] == '#')
                for (int k = i - 1; k > max(i - K, 0); k--)
                    B[k][j] = '#';

    int sum = 0;

    if (K == 1)
    {
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++)
                if (A[i][j] == '.') sum++;
        cout << sum;
        return 0;
    }

    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
        {
            if (A[i][j] == '.') sum++;
            if (B[i][j] == '.') sum++;
        }
    cout << sum;
    return 0;
}