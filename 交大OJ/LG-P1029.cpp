#include <bits/stdc++.h>

using namespace std;

int main()
{
    int x0, y0; cin >> x0 >> y0;
    if (y0 % x0 != 0) { cout << 0; return 0;}

    int D = y0 / x0;

    bool flag;
    int sum = 0;

    flag = false;
    while(D % 2 == 0)
    {
        D /= 2;
        flag = true;
    }
    if (flag) sum++;

    for (int i = 3; D != 1; i += 2)
    {
        flag = false;
        while(D % i == 0)
        {
            D /= i;
            flag = true;
        }
        if (flag) sum++;
    }

    cout << pow(2.0, sum);
    return 0;
}