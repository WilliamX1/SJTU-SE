#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
long long L, R;
const long long N = 1000010;
long long Prime[N];
bool Check[N];
int tot = 0;

void getPrime()
{
    memset(Check, 0, sizeof(Check));

    for (long long i = 2; i < N; i++)
    {
        if (!Check[i])
            Prime[++tot] = i;
        for (long long j = 1; j <= tot; j++)
        {
            if (i * Prime[j] > N)
                break;
            Check[i * Prime[j]] = true;
            if (i % Prime[j] == 0)
                break;
        }
    }
    //for (int i = 1; i <= tot; i++)
        //cout << Prime[i] << " ";

}
bool isPrime[N];

int main()
{
    getPrime();
    cin >> L >> R;
    memset(isPrime, 0, sizeof(isPrime));

    for (long long i = 1; i <= tot; i++)
    {
        long long start;
        if (L / Prime[i] <= 1)
            start = 2 * Prime[i];
        else
        {
            start = (L / Prime[i]) * Prime[i];
            while(start < L) start += Prime[i];
        }

        for (long long j = start; j <= R; j += Prime[i])
            isPrime[j - L] = true;
    }

    int res = 0;
    for (int i = 0; i <= R - L; i++)
        if (!isPrime[i]) res++;
    cout << res;
    return 0;
}