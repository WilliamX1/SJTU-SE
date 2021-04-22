#include <bits/stdc++.h>
using namespace std;

string A[25];

int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++) cin >> A[i];

    for (int i = 1; i < n; i++)
    {
        for (int j = i + 1; j <= n; j++)
            if (A[j] + A[i] > A[i] + A[j])
                swap(A[j], A[i]);
    }
    for (int i = 1; i <= n; i++) cout << A[i];
    return 0;
}