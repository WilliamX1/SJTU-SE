#include <iostream>

using namespace std;
long long fastPower(long long base, long long k, long long mod)
{
    long long res = 1;
    while(k > 0)
    {
        if (k & 1)
            res = (res * base) % mod;
        k = k >> 1;
        base = (base * base) % mod;
    }
    return res % mod;
}
int main()
{
    long long base, k, mod; cin >> base >> k >> mod;
    cout << base << '^' << k << " mod " << mod << '=' << fastPower(base, k, mod);
    return 0;
}