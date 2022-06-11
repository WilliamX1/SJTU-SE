#include <bits/stdc++.h>

using namespace std;

int T[30];
bool is(int x)
{
    if (x == 0 || x == 1) return false;
    for (int i = 2; i <= sqrt(x); i++)
        if (x % i == 0) return false;
    return true;
}
int main()
{
    string s; cin >> s;
    memset(T, 0, sizeof(T));

    int M = 0, m = 1e9;
    int len = s.length();
    for (int i = 0; i < len; i++)
        T[s[i] - 'a']++;

    for (int i = 0; i < 30; i++)
        if (T[i])
        {
            if (T[i] > M) M = T[i];
            else if (T[i] < m) m = T[i];
        }
    if (is(M - m))
        cout << "Lucky Word" << endl << M - m;
    else
        cout << "No Answer" << endl << 0;

    return 0;
}