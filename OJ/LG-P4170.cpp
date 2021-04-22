#include <bits/stdc++.h>

using namespace std;
int dp[52][52];

int main()
{
    string A; cin >> A;
    int len = A.length();
    A = ' ' + A;

    memset(dp, 0x7f, sizeof(dp));

    for (int k = 0; k < len; k++)
    {
        int l = 1, r;
        for (; r = l + k, r <= len; l++)
        {
            for (int j = l; j <= r; j++)
                if (l == r)
                    dp[l][l] = 1;
                else if (A[l] == A[r]) dp[l][r] = min(dp[l + 1][r], dp[l][r - 1]);
                else dp[l][r] = min(dp[l][r], dp[l][j] + dp[j + 1][r]);
        }
    }
    cout << dp[1][len];
    return 0;
}
