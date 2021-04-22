#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    double n, m; cin >> n >> m;
    double x = exp(log(n) / m);

    if (abs(x - int(x)) < 1e-6) cout << int(x);
    else if (abs(x - int(x) - 1) < 1e-6) cout << int(x) + 1;
    else cout << int(x);
    return 0;
}