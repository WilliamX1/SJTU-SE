#include <bits/stdc++.h>

using namespace std;

int n, r, A[100];

void Dfs(int k)
{
    int i;
    if (k > r)
    {
        for (i = 1; i <= r; i++)
            cout << setw(3) << A[i];
        cout << "\n";
        return;
    }

    for (i = A[k - 1] + 1; i <= n; i++)
    {
        A[k] = i;
        Dfs(k + 1);
    }
}
int main()
{
    cin >> n >> r;
    A[0] = 0;

    Dfs(1);
    return 0;
}