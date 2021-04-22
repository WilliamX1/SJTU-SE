#include <bits/stdc++.h>

using namespace std;

int main()
{
    int M, S, T; cin >> M >> S >> T;

    int sum = 0;
    int i = 1;
    for (; i <= T; i++)
    {
        if (M >= 10) { sum += 60; M -= 10;}
        else if (((10 - M - 1) / 4 + 1 + 1) * 17 >= S - sum) sum += 17;
        else if (M + 4 * (T - i) >= 10)
            M += 4;
        else sum += 17;

        if (sum >= S) { cout << "Yes\n" << i; break;}
    }
    if (i > T) cout << "No\n" << sum;

    return 0;
}