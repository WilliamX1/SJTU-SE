#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[128];

int main()
{
    int N; scanf("%d", &N);

    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);
    int ave = accumulate(A + 1, A + N + 1, 0) / N;

    int sum = 0;
    for (int i = 1; i < N; i++)
        if (A[i] < ave)
        {
            sum++;
            A[i + 1] -= ave - A[i];
        }
        else if (A[i] > ave)
        {
            sum++;
            A[i + 1] += A[i] - ave;
        }
    printf("%d", sum);
    return 0;
}