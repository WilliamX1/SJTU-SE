#include <bits/stdc++.h>

using namespace std;

bool T[10010];
int main()
{
    int l, m; cin >> l >> m;
    int left, right;
    while(m--)
    {
        cin >> left >> right;
        for (int i = left; i <= right; i++)
            T[i] = 1;
    }
    cout << l + 1 - accumulate(T, T + l + 1, 0);

    return 0;
}