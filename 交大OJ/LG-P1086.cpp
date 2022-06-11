#include <bits/stdc++.h>
using namespace std;
struct node
{
    int x;
    int y;
    int num;
};
node T[410];
int T_xb = 0;
bool myCompare(node a, node b)
{
    return a.num > b.num;
}
int A[21][21];

int main()
{
    int M, N, K; cin >> M >> N >> K;
    for (int i = 1; i <= M; i++)
        for (int j = 1; j <= N; j++)
        {
            cin >> A[i][j];
            if (A[i][j] != 0)
            {
                ++T_xb;
                T[T_xb].x = i;
                T[T_xb].y = j;
                T[T_xb].num = A[i][j];
            }
        }
    sort(T + 1, T + T_xb + 1, myCompare);

    T[0].x = 0, T[0].y = T[1].y;
    int sum = 0;
    for (int i = 1; i <= T_xb; i++)
    {
        int time = abs(T[i].y - T[i - 1].y) + abs(T[i].x - T[i - 1].x) + 1 + T[i].x;
        if (K >= time)
        {
            sum += T[i].num;
            K -= time - T[i].x;
        }
        else break;
    }
    cout << sum;
    return 0;
}