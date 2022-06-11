#include <iostream>

using namespace std;

int main()
{
    long long n; cin >> n;

    long long index = 0;
    while(n > 0)
    {
        n <<= 1;
        index++;
    }

    index = 64 - index;
    //cout << index << " ";
    long long res = 1;
    res <<= index;
    res--; res <<= 1;

    cout << res;
    return 0;
}