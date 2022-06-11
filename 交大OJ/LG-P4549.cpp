#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[21];
int gcd(int a, int b)
{
    int tmp;
    while(b)
    {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}
int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &A[i]);
        A[i] = abs(A[i]);
    }
    int res = gcd(A[1], A[2]);
    for (int i = 3; i <= n; i++)
        res = gcd(res, A[i]);
    printf("%d", res);

    return 0;
}