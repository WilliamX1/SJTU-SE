#include <bits/stdc++.h>
using namespace std;

int A[10];
int n;

void Dfs(int i, int x)
{
    A[i] = x;

    if (i == n)
    {
        for (int j = 1; j <= n; j++)
            cout << setw(5) << A[j];
        cout << endl;
        return;
    }

    bool flag = true;
    for (int j = 1; j <= n; j++)
    {
        flag = true;
        for (int k = 1; k <= i; k++)
            if (A[k] == j) {flag = false; break;};
        if (flag) Dfs(i + 1, j);
    }
    return;
}
int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
        Dfs(1, i);
    return 0;
}