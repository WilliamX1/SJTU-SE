#include <bits/stdc++.h>

using namespace std;

struct node
{
    int L;
    int R;
};
node A[20010];
int B[20010][2];  //[0]代表从线段左边开始走，[1]相反

int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i].L >> A[i].R;
    A[0].L = A[0].R = 1;


    memset(B, 0, sizeof(B));
    for (int i = 1; i <= n; i++)
    {
        B[i][0] = A[i].R - A[i].L + min(B[i - 1][0] + abs(A[i - 1].R - A[i].L), B[i - 1][1] + abs(A[i - 1].L - A[i].L));
        B[i][1] = A[i].R - A[i].L + min(B[i - 1][0] + abs(A[i - 1].R - A[i].R), B[i - 1][1] + abs(A[i - 1].L - A[i].R));
    }

    cout << min(B[n][0] + n - A[n].R, B[n][1] + n - A[n].L) + n - 1;
    return 0;
}