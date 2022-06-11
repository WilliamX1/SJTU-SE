#include <bits/stdc++.h>

using namespace std;

long long int C[64];

int main()
{
    string s; cin >> s;
    int len = s.length();
    long long int N; cin >> N;

    C[0] = len;
    for (int i = 1; i < 63; i++)
        C[i] = 2 * C[i - 1];

    while(N > len)
    {
        int i = 0;
        for (; i < 63; i++)
            if (C[i] < N && N <= C[i + 1])
                break;
        if (N == C[i] + 1) N--;
        else
            N -= C[i] + 1;
    };
    cout << s[N - 1];

    return 0;
}