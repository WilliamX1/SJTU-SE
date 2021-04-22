#include <bits/stdc++.h>

using namespace std;

int f(int x)
{
    if (x == 0) return 6;
    if (x == 1) return 2;
    if (x == 2) return 5;
    if (x == 3) return 5;
    if (x == 4) return 4;
    if (x == 5) return 5;
    if (x == 6) return 6;
    if (x == 7) return 3;
    if (x == 8) return 7;
    if (x == 9) return 6;
    if (x >= 10)
        return f(x / 10) + f(x % 10);
}
int main()
{
    int n; cin >> n;
    n -= 4;  //除去加号和等号

    int k, sum = 0;
    for (int i = 0; i <= 1000; i++)
        for (int j = 0; j <= 1000; j++)
        {
            int k = i + j;
            if (f(i) + f(j) + f(k) == n)
            {
                //cout << i << " + " << j << " = " << k << endl;
                sum++;
            }

        }
    cout << sum;

    return 0;
}