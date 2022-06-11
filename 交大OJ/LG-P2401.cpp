#include <iostream>

using namespace std;

int A[1024][1024];
const int Mod = 2015;
int main()
{
    int n, k; cin >> n >> k;

    A[1][0] = 0;
    A[2][0] = A[2][1] = 1;
    for (int i = 3; i <= n; i++)
        for (int j = 0; j < i; j++)
            A[i][j] = (A[i - 1][j] * (j + 1) + A[i - 1][j - 1] * (i - j)) % Mod;
    cout << A[n][k];
    return 0;
}