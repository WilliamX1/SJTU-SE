#include <bits/stdc++.h>
using namespace std;

int C(int bottom, int top)
{
    int res = 1;
    for (int i = 0; i < top; i++)
        res *= bottom - i;
    for (int i = 0; i < top; i++)
        res /= top - i;
    return res;
}
int A[12];
int B[10000][12];

int main()
{
    int n; cin >> n;
    if (n > 30) { cout << 0; return 0;};

    int sum = 0, tmp;
    for (A[1] = 1; A[1] <= 3; A[1]++)
        for (A[2] = 1; A[2] <= 3; A[2]++)
            for (A[3] = 1; A[3] <= 3; A[3]++)
                for (A[4] = 1; A[4] <= 3; A[4]++)
                    for (A[5] = 1; A[5] <= 3; A[5]++)
                        for (A[6] = 1; A[6] <= 3; A[6]++)
                            for (A[7] = 1; A[7] <= 3; A[7]++)
                                for (A[8] = 1; A[8] <= 3; A[8]++)
                                    for (A[9] = 1; A[9] <= 3; A[9]++)
                                        for (A[10] = 1; A[10] <= 3; A[10]++)
                                            {
                                                tmp = 0;
                                                for (int i = 1; i <= 10; i++)
                                                    tmp += A[i];
                                                if (tmp == n)
                                                {
                                                    sum++;
                                                    for (int j = 1; j <= 10; j++)
                                                        B[sum][j] = A[j];
                                                }
                                            }

    cout << sum << endl;
    for (int i = 1; i <= sum; i++)
    {
        for (int j = 1; j <= 10; j++)
            cout << B[i][j] << " ";
        cout << endl;
    }
    return 0;
}