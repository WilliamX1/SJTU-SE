#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int N = 60;
long long Fib[N];
int main()
{
    int t; cin >> t;
    while(t--)
    {
    int n; scanf("%d", &n);
    cout << n << "=";
    Fib[0] = 0; Fib[1] = 1;

    for (int i = 2; i <= 51; i++)
        Fib[i] = Fib[i - 1] + Fib[i - 2];

    long long res[100]; int index = 0;
    for (int i = 51; i >= 1; i--)
    {
        if (n >= Fib[i])
        {
            res[++index] = Fib[i];
            n -= Fib[i];
        }
        if (n == 0)
            break;
    }
    cout << res[index];
    for (int i = index - 1; i >= 1; i--)
        cout << "+" << res[i];
    cout << endl;
    }
    return 0;
}