#include <bits/stdc++.h>

using namespace std;
/*
就是求最长公共子序列
*/
int dp[2048][2048];

int main()
{
    string A, B; cin >> A >> B;
    int len_A = A.length(), len_B = B.length();
    A = 'A' + A, B = 'B' + B;

    memset(dp, 0, sizeof(dp));
    for (int i = 0; i <= len_A; i++) dp[i][0] = i;
    for (int j = 0; j <= len_B; j++) dp[0][j] = j;

    for (int i = 1; i <= len_A; i++)
        for (int j = 1; j <= len_B; j++)
            if (A[i] == B[j])
                dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
    /*for (int i = 1; i <= len_A; i++)
    {
        for (int j = 1; j <= len_B; j++)
            cout << dp[i][j] << " ";
        cout << endl;
    }*/
    cout << dp[len_A][len_B];
    return 0;
}