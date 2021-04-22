#include <bits/stdc++.h>

using namespace std;

bool A[1010];

int main()
{
    int n; cin >> n;
    int tmp, sum = 0;

    memset(A, 0, sizeof(A));
    for (int i = 0; i < n; i++)
    {
        cin >> tmp;
        if (A[tmp] == 0)
            sum++;
        A[tmp] = 1;
    }
    cout << sum << endl;
    for (int i = 1; i <= 1000; i++)
        if (A[i]) cout << i << " ";

    return 0;
}