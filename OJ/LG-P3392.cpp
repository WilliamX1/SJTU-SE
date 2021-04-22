#include <bits/stdc++.h>

using namespace std;

int A[52][52];

int main()
{
    int N, M; cin >> N >> M;

    memset(A, 0, sizeof(A));
    char ch;
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= M; j++)
    {
        cin >> ch;
        if (ch == 'W') A[i][j] = 0; //白色
        if (ch == 'B') A[i][j] = 1; //蓝色
        if (ch == 'R') A[i][j] = 2; //红色
    }

    int sum = 0, k = N, res = 1e9;

    for (int i = 1; i <= N - 2; i++)
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum = 0;
            for (int s = 1; s <= i; s++)
                for (int t = 1; t <= M; t++)
                    if (A[s][t] != 0) sum++;
            for (int s = i + 1; s <= j; s++)
                for (int t = 1; t <= M; t++)
                    if (A[s][t] != 1) sum++;
            for (int s = j + 1; s <= k; s++)
                for (int t = 1; t <= M; t++)
                    if (A[s][t] != 2) sum++;
            if (sum <= res) res = sum;
        }
    cout << res;
    return 0;
}