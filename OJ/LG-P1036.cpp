#include <bits/stdc++.h>

using namespace std;

int A[25];

bool is(int x)
{
    if (x == 0 || x == 1) return false;
    if (x == 2) return true;
    for (int i = 2; i <= sqrt(x) + 1; i++)
        if (x % i == 0) return false;
    return true;
}
int main()
{
    int n, k; cin >> n >> k;

    for (int i = 1; i <= n; i++)
        cin >> A[i];

    int x, sum, sum_xb, res = 0;
    for (int i = 0; i < pow(2.0, double(n)); i++)
    {
        sum = sum_xb = 0;
        x = i;
        for (int j = 1; j <= n; j++)
        {
            if (x % 2)
            {
                sum_xb++;
                sum += A[j];
            }
            x = x >> 1;
        }
        if (sum_xb == k && is(sum)) res++;
    }

    cout << res;
    return 0;
}