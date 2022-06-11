#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[128];
int dp_L[128], dp_R[128];

int main()
{
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);

    memset(dp_L, 0, sizeof(dp_L));
    memset(dp_R, 0, sizeof(dp_R));

    for (int i = 1; i <= N; i++)
        for (int j = i - 1; j >= 0; j--)
            if (A[j] < A[i])
                dp_L[i] = max(dp_L[i], dp_L[j] + 1);
    for (int i = N; i >= 1; i--)
        for (int j = i + 1; j <= N + 1; j++)
            if (A[j] < A[i])
                dp_R[i] = max(dp_R[i], dp_R[j] + 1);
    int res = 0;
    for (int i = 1; i <= N; i++)
        res = max(res, dp_L[i] + dp_R[i+ 1]);
    cout << N-res;
    return 0;
}