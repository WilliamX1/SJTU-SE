#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[2000010];
int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    while(m--)
    {
        scanf("%d", &n);
        printf("%d\n", A[n]);
    }
    return 0;
}