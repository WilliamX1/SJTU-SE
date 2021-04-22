#include <iostream>

using namespace std;

int main()
{
    double S = 0;
    double k; cin >> k;
    double i = 1;
    for (; S <= k; i++)
        S += 1 / i;
    cout << int(i - 1);
    return 0;
}
