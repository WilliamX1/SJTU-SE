#include <iostream>

using namespace std;
int dp[21];

int main()
{
    int h, k; cin >> h >> k;

    cout << (k + 1) * (1 << h);

    return 0;
}