#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[100010];

int main()
{
    int n; scanf("%d", &n);

    A[0] = 0;
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);

        int sum = 0;
    for (int i = 1; i <= n; i++)
        if (A[i] > A[i - 1])
            sum += A[i] - A[i - 1];
    printf("%d", sum);
    return 0;
}