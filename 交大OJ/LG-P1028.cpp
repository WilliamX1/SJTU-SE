#include <bits/stdc++.h>

using namespace std;

int dp[1024];
int main()
{
    int n; cin >> n;

    dp[0] = dp[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        if (i % 2 == 1)
            dp[i] = dp[i - 1];
        else
            dp[i] = dp[i - 1] + dp[i / 2];
    }
    cout << dp[n];

    return 0;
}