#include <iostream>

using namespace std;

int Fib[40000];
int main()
{
    int N; cin >> N;

    Fib[0] = 0, Fib[1] = 1;
    for (int i = 2; i < 40000; i++)
        Fib[i] = i + Fib[i - 1];

    int x;
    for (int j = 0; j < 40000; j++)
        if (Fib[j] < N && N <= Fib[j + 1])
        { x = j; break;}

    int sun = x + 1 - (N - Fib[x] - 1);
    cout << sun << '/' << x + 2 - sun;

    return 0;
}