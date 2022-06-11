#include <bits/stdc++.h>

using namespace std;

int A[32];

int T[20010];
int T_xb = 0;

bool flag[20010];

int main()
{
    int n, V; cin >> V >> n;
    memset(A, 0, sizeof(A));
    memset(flag, 0, sizeof(flag));

    for (int i = 1; i <= n; i++)
        cin >> A[i];

    int x;
    for (int i = 1; i <= n; i++)
    {
        int tmp_xb = T_xb;
        for (int j = 1; j <= tmp_xb; j++)
        {
            x = T[j] - A[i];
            if (x >= 0 && !flag[x])
            {
                T[++T_xb] = x;
                flag[x] = true;
            }
        }
        x = V - A[i];
        if (x >= 0 && !flag[x])
        {
            T[++T_xb] = x;
            flag[x] = true;
        }
    }
    for (int i = 0; i <= V; i++)
        if (flag[i]) {cout << i; break;}
    return 0;
}