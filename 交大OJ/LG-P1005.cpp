#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

__int128 cifang[100];
__int128 A[100];
__int128 dp[100][100];

inline int read()
{
    int x;
    cin >> x;
    return x;
}
inline void print(__int128 x)
{
    if (x > 9) print(x / 10);
    putchar(x % 10 + '0');
}

int main()
{
    int n, m; cin >> n >> m;

    cifang[0] = 1;
    for (int i = 1 ; i < 100; i++)
        cifang[i] = 2 * cifang[i - 1];

    __int128 sum = 0;

    while(n--)
    {
        for (int i = 1; i <= m; i++)
            A[i] = read();

        dp[0][m + 1] = 0;

        for (int i = m; i >= 1; i--)  //i + 1代表区间长度
        {
            for (int j = 0; j <= m - i + 1; j++)  //j代表左端点
            {
                int left = j;
                int right = j + i;
                if (left == 0) dp[left][right] = dp[left][right + 1] + A[right] * cifang[m - i + 1];
                else if (right == m + 1) dp[left][right] = dp[left - 1][right] + A[left] * cifang[m - i + 1];
                else
                    dp[left][right] = max(A[left] * cifang[m - i + 1] + dp[left - 1][right], A[right] * cifang[m - i + 1] + dp[left][right + 1]);
            }
        }

        __int128 res = 0;
        for (int i = 0; i <= m; i++)
            res = max(res, dp[i][i + 1]);
        //cout << res << endl;
        sum += res;
    }
    print(sum);
    return 0;
}