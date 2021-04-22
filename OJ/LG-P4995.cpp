#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[400];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    A[0] = 0;
    int l = 0, r = n;

    sort(A, A + n + 1);
    long long int sum = 0;
    int dir = 1;
    while(l != r)
    {
        sum += (long long)(A[r] - A[l]) * (long long)(A[r] - A[l]);
        if (dir == 1) { l++; dir = -1;}
        else { r--; dir = 1;}
    }
    printf("%lld", sum);
    return 0;
}