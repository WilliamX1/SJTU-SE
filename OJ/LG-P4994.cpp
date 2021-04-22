#include <iostream>

using namespace std;

int A[10000000];

int main()
{
    int M; cin >> M;

    if (M == 1) { cout << 0; return 0;}

    A[0] = 0;
    A[1] = 1;
    for (int i = 2; ; i++)
    {
        A[i] = (A[i - 1] + A[i - 2]) % M;
        if (A[i] == 1 && A[i - 1] == 0)
            { cout << i - 1; return 0;}
    }
    return 0;
}