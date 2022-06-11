#include <bits/stdc++.h>

using namespace std;

int A[1024];
bool T[100000010];

int main()
{
    int n; cin >> n;


    for (int i = 0; i < n; i++)
        cin >> A[i];

    int sum = 0;
    memset(T, 0, sizeof(T));
    for (int i = 1; i < n; i++)
    {
        int x = abs(A[i] - A[i - 1]);
        if (T[x] == 0) { T[x] = 1; sum++;};
    }

    if (sum == n - 1) cout << "Jolly";
    else cout << "Not jolly";

    return 0;
}