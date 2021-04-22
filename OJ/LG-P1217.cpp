#include <bits/stdc++.h>

using namespace std;

bool isH(int s[], int len)
{
    for (int i = 1; i <= len / 2; i++)
        if (s[i] != s[len - i + 1]) return false;
    return true;
}
int main()
{
    int a, b; cin >> a >> b;

    bool flag;
    int len = 0;
    int s[10];

    b = min(b, 9989900);
    for (int i = a; i <= b; i++)
    {
        int tmp = i;

        len = 0;
        while(tmp > 0)
        {
            s[++len] = tmp % 10;
            tmp /= 10;
        }

        if(isH(s, len))
        {
            flag = 1;
            for (int j = 2; j <= sqrt(i) + 1; j++)
                if (i % j == 0) {flag = false; break;}
            if (flag) cout << i << endl;
        }

    }
    return 0;
}