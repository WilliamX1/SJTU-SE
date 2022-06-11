#include <bits/stdc++.h>

using namespace std;
const int N = 1000000 + 10;
int Prime[N];
bool Check[N];
void getPrime()
{
    int tot = 0;
    memset(Check, 0, sizeof(Check));
    for (int i = 2; i < N; i++)
    {
        if (!Check[i])
            Prime[++tot] = i;
        for (int j = 1; j <= tot; j++)
        {
            if (i * Prime[j] > N)
                break;
            Check[i * Prime[j]] = true;
            if (i % Prime[j] == 0)
                break;
        }
    }
}
int main()
{
    getPrime();
    int n; cin >> n;
    int res = 0;
    int tmp_sum;
    int tmp_res;
    for (int i = 1; i <= n; i++)
    {
        int m = i;
        tmp_res = 1;
        for (int j = 1; m > 1; j++)
        {
            if (!Check[m])
            {
                tmp_res *= 2;
                break;
            }
            tmp_sum = 0;
            while(m % Prime[j] == 0)
            {
                m /= Prime[j];
                tmp_sum++;
            }
            tmp_res *= tmp_sum + 1;
        }
        res += tmp_res;
    }
    cout << res;
    return 0;
}