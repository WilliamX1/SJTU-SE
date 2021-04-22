#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[100010];

int main()
{
    int n, x; scanf("%d%d", &n, &x);
    for (int i = 0; i < n; i++) scanf("%d", &A[i]);

    long long int sum = 0;
    if (A[0] > x) { sum = A[0] - x; A[0] = x;};

    for (int i = 1; i < n; i++)
        if (A[i] + A[i - 1] > x)
        {
            sum += (long long)A[i] + (long long)A[i - 1] - (long long)x;
            A[i] = x - A[i - 1];
        }
    cout << sum;

    return 0;
}