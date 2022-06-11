#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[1010];
int q[1010];

int main()
{
    int M, N; scanf("%d%d", &M, &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);

    int l = 1, r = 0;
    int sum = 0;
    bool flag;

    for (int i = 1; i <= N; i++)
    {
        flag = false;
        for (int j = r; j >= l; j--)
            if (q[j] == A[i])
            {
                flag = true;
                break;
            }
        if (!flag)
        {
            sum++;
            if (r - l + 1 == M)
                l++;
            q[++r] = A[i];
        }
    }
    printf("%d", sum);

    return 0;
}