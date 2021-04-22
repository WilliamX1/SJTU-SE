#include <bits/stdc++.h>

using namespace std;
int A[800];
int dp[800][800][2];

int main()
{
    int N; cin >> N;
    string str; cin >> str;
    for (int i = 0; i < N; i++)
        switch(str[i])
        {
            case 'w': A[i + 1] = A[i + 1 + N] = 0; break;
            case 'r': A[i + 1] = A[i + 1 + N] = 1; break;
            case 'b': A[i + 1] = A[i + 1 + N] = 2; break;
        }
    //for (int i = 1; i <= N; i++) cout << A[i] << " ";
    N <<= 1;
    int res = 0;

        memset(dp, 0, sizeof(dp));
        for (int step = 0; step < N; step++)
            for (int l = 1; l <= N; l++)
                if (!step) dp[l][l][0] = dp[l][l][1] = 1;
                else
                {
                    int r = l + step;
                    //dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);

                    if (!A[l])  dp[l][r][0] = dp[l + 1][r][0] + 1;
                    else{
                        int index = l + 1;
                        while(index <= r && !A[index]) index++;

                        if (index <= r && A[l] == A[index])
                            dp[l][r][0] = index - l + dp[index][r][0];
                        else dp[l][r][0] = index - l;
                    }

                    if (!A[r]) dp[l][r][1] = dp[l][r - 1][1] + 1;
                    else{
                        int index = r - 1;
                        while(index >= l && !A[index]) index--;

                        if (index >= l && A[r] == A[index])
                            dp[l][r][1] = r - index + dp[l][index][1];
                        else dp[l][r][1] = r - index;
                    }
                }
        A[0] = A[N];
        for (int i = N; i > 0; i--)
            A[i] = A[i - 1];

        for (int l = 1; l <= N; l++)
            for (int r = l; r <= N && r - l < (N >> 1); r++)
                for (int m = l; m <= r; m++)
                    res = max(res, dp[l][m][1] + dp[m + 1][r][0]);

    cout << res;

    return 0;
}