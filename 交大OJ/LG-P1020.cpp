#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[100010];
int dp_lower[100010];
int dp_higher[100010];
int main()
{
    int n = 1;
    while(cin >> A[n]) n++;

    int dp_index = 0;
    dp_lower[++dp_index] = A[1];
    for (int i = 2; i < n; i++)
    {
        if (A[i] <= dp_lower[dp_index])
            dp_lower[++dp_index] = A[i];
        else
        {
            int index = upper_bound(dp_lower + 1, dp_lower + dp_index + 1, A[i], greater<int>()) - dp_lower;
            dp_lower[index] = A[i];
        }
    }
    cout << dp_index << endl;

    dp_index = 0;
    dp_higher[++dp_index] = A[1];
    for (int i = 2; i < n; i++)
    {
        if (A[i] > dp_higher[dp_index])
            dp_higher[++dp_index] = A[i];
        else
        {
            int index = lower_bound(dp_higher + 1, dp_higher + dp_index + 1, A[i]) - dp_higher;
            dp_higher[index] = A[i];
        }
    }
    cout << dp_index;
    return 0;
}