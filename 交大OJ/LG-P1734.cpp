#include <bits/stdc++.h>

using namespace std;
int A[1024];
int dp[1024];
int main()
{
    int S; cin >> S;
    memset(A, 0, sizeof(A));

    for (int i = 1; i <= S; i++)
        for (int j = 1; j < i; j++)
            if (i % j == 0) A[i] += j;
    //for (int i = 1; i <= S; i++) cout << A[i] << " ";

    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= S; i++)
        for (int j = S; j >= i; j--)
            dp[j - i] = max(dp[j - i], dp[j] + A[i]);
    cout << dp[0];
    return 0;
}