#include <bits/stdc++.h>
using namespace std;

int A[101][101];
int B[101][101];

int main()
{
    int n, m, x, y; cin >> n >> m >> x >> y;
    int x1, y1, x2, y2;
    memset(A, 0, sizeof(A));

    for (int i = 1; i <= x; i++)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        for (int j = x1; j <= x2; j++)
            for (int k = y1; k <= y2; k++)
            {
                A[j][k]++;
                B[j][k] = i;
            }
    }
    for (int i = 1; i <= y; i++)
    {
        cin >> x1 >> y1;
        if (A[x1][y1])
            cout << "Y " << A[x1][y1] << " " << B[x1][y1] << endl;
        else
            cout << "N" << endl;
    }
    return 0;
}