#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    string a; cin >> a;

    int last;
    int A[6]; memset(A, 0, sizeof(A));

    if (a[0] == 'M' && a[1] == 'D' && a[2] == 'A')
    {
        for (int i = 7; i >= 3; i--)
            if (a[i] >= '0' && a[i] <= '9') { last = a[i] - '0'; break;}
        if (last == 1 || last == 9) A[1] = 1;
        else if (last == 2 || last == 8) A[2] = 1;
        else if (last == 3 || last == 7) A[3] = 1;
        else if (last == 4 || last == 6) A[4] = 1;
        else if (last == 5 || last == 0) A[5] = 1;
    }
    else
        A[1] = A[2] = A[3] = A[4] = A[5] = 1;

    cout << A[1] << " " << A[2] << " " << A[3] << " " << A[4] << " " << A[5];

    return 0;
}