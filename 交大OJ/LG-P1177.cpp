#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

int A[100010];

int main()
{
    int n; cin >> n;

    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);
    sort(A, A + n);
    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);
    return 0;
}