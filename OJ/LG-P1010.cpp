#include <iostream>

using namespace std;

int A[20] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

void f(int x)
{
    if (x == 0) return;
    for (int i = 0; i < 20; i++)
        if (x >= A[i] && x < A[i + 1])
        {
            if (i == 0)
                cout << "2(0)";
            else if (i == 1)
                cout << "2";
            else if (i == 2)
                cout << "2(2)";
            //else if (i == 2 || i == 4 || i == 8 || i == 16)
                //cout << 2 << '(' << i << ')';
            else
            {
                cout << "2(";
                f(i);
                cout << ")";
            }
            if (x != A[i])
            {
                cout << '+';
                f(x - A[i]);
            }
            break;
        }
    return;
}

int main()
{
    int N; cin >> N;
    /*for (int i = 1; i < 20000; i++)
        {
            f(i);
           cout << '\n';
        }*/
    f(N);
    return 0;
}