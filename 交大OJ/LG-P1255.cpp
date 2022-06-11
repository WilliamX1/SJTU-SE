#include <bits/stdc++.h>
#include <cstring>
using namespace std;
const int N = 1200;
int dp[5010][N + 10];

void bigplus(int x, int* A, int* B)
{
    int yushu = 0;
    for (int i = 1; i <= N; i++)
    {
        dp[x][i] = A[i] + B[i] + yushu;
        if (dp[x][i] > 9)
        {
            dp[x][i] -= 10;
            yushu = 1;
        }else yushu = 0;
    }
}
int main()
{
    int n; cin >> n;
    if (n == 0) {cout << 0; return 0;};

    memset(dp, 0, sizeof(dp));
    dp[0][1] = 1;
    dp[1][1] = 1;
    for (int i = 2; i <= n; i++)
        bigplus(i, dp[i - 1], dp[i - 2]);

    int index = N;

    while(!dp[n][index])
        index--;
    for (int i = index; i >= 1; i--)
        cout << dp[n][i];

    return 0;
}