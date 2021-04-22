#include <bits/stdc++.h>

using namespace std;

int A[1000010];
int O[1000010];

int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i];
    //所有人初始化为1
    for (int i = 1; i <= n; i++)
        O[i] = 1;
    //正向不减子序列
    for (int i = 2; i <= n; i++)
    {
        if (A[i] > A[i - 1]) O[i] = O[i - 1] + 1;
        else if (A[i] == A[i - 1]) O[i] = O[i - 1];
    }
    //反向不减子序列
    for (int i = n - 1; i >= 1; i--)
    {
        if (A[i] > A[i + 1]) O[i] = max(O[i], O[i + 1] + 1);
        else if (A[i] == A[i + 1]) O[i] = O[i + 1];
    }

    long long res = 0;
    for (int i = 1; i <= n; i++)
        res += O[i];
    cout << res;
    return 0;
}