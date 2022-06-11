#include <bits/stdc++.h>

using namespace std;
int A[200][200];
int dp[200][200];

int main()
{
    int n; cin >> n;
    memset(A, 0, sizeof(A));
    for (int i = 1; i < n; i++)
        for (int j = i + 1; j <= n; j++)
            cin >> A[i][j];

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            if (i >= j) dp[i][j] = 0;
            else dp[i][j] = A[i][j];

    int r;
    for (int step = 1; step < n; step++)
        for (int l = 1; r = l + step, r <= n; l++)
            for (int m = l; m < r; m++)
                dp[l][r] = min(dp[l][r], min(dp[l][m] + dp[m + 1][r] + A[m][m + 1], dp[l][m] + A[m][r]));

    /*for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            cout << dp[i][j] << "\t";
        cout << endl;
    }*/
    cout << dp[1][n];
    return 0;
}