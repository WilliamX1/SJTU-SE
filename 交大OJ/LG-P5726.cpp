#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[1024];

int main()
{
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);
    sort(A + 1, A + N + 1);

    int res = 0;
    for (int i = 2; i < N; i++)
        res += A[i];
    printf("%.2f", double(res) / (N - 2));
    return 0;
}