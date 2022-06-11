#include <bits/stdc++.h>

using namespace std;

int A[20];
int dp[100001];

int main()
{
    int n; cin >> n;

    memset(dp, 0x7f, sizeof(dp));
    int top = sqrt(sqrt(n));
    for (int i = 1; i <= top; i++)
    {
        A[i] = i * i * i * i;
        dp[i * i * i * i] = 1;
    }

    for (int i = 1; i <= top; i++)
        for (int j = A[i]; j <= n; j++)
            dp[j] = min(dp[j], dp[j - A[i]] + 1);
    cout << dp[n];
    return 0;
}
