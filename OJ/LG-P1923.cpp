#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

int A[5000010];

int main()
{
    int n, k; cin >> n >> k;
    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);
    nth_element(A, A + k, A + n);
    cout << A[k];
    return 0;
}