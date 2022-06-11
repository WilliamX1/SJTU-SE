#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[5010][5010];  //0代表无关系，1代表i被j吃，2代表i吃j

int eat[5010];
int beEaten[5010];
long long int dp[5010];
bool T[5010];
const long long Mod = 80112002;

int main()
{
    int n, m; scanf("%d%d", &n, &m);

    memset(A, 0, sizeof(A));
    memset(T, 0, sizeof(T));
    memset(dp, 0, sizeof(dp));
    memset(eat, 0, sizeof(eat));
    memset(beEaten, 0, sizeof(beEaten));

    int x, y;
    while(m--)
    {
        scanf("%d%d", &x, &y);
        A[x][y] = 1; beEaten[x]++;
        A[y][x] = 2; eat[y]++;
    }

    for (int i = 1; i <= n; i++)
        if (!eat[i]) dp[i] = 1;

    int S[5000], s_xb = 0;
    for (int i = 1; i <= n; i++)
        if (!beEaten[i]) S[++s_xb] = i;

    int ans = n;
    while(ans > 0)
    {
        bool flag = false;
        for (int i = 1; i <= n; i++)
            if (!T[i] && !eat[i])
            {
                flag = true;
                for (int j = 1; j <= n; j++)
                    if (A[i][j] == 1)
                    {
                        dp[j] += dp[i]; dp[j] %= Mod;
                        A[i][j] = A[j][i] = 0;
                        beEaten[i]--;
                        eat[j]--;
                    }
                T[i] = true;
                ans--;
            }
        if (!flag) break;
    }

    long long int res = 0;
    for (int i = 1; i <= s_xb; i++)
    {
        res += dp[S[i]];
        res %= Mod;
    }
    cout << res;

    return 0;
}
