#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[30010];
int main()
{
    int w, n; scanf("%d%d", &w, &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);

    sort(A + 1, A + n + 1);

    int l = 1, r = n;
    int sum = 0;
    while(r > l)
    {
        if (A[r] + A[l] <= w) l++;
        sum++; r--;
    }
    if (r == l) sum++;
    cout << sum;

    return 0;
}