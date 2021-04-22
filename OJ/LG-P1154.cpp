#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[5010];
bool T[1000010];

int main()
{
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);

    memset(T, 0, sizeof(T));
    for (int i = 1; i <= N; i++)
        for (int j = i + 1; j <= N; j++)
            if (abs(A[j] - A[i]) <= 1000000)
                T[abs(A[j] - A[i])] = 1;

    bool F[1000010];
    bool flag;
    int i = N;
    while(1)
    {
        if (!T[i])
        {
            memset(F, 0, sizeof(F));
            flag = true;
            for (int j = 1; j <= N; j++)
                if (F[A[j] % i]) { flag = false; break;}
                else F[A[j] % i] = 1;
            break;
        }
        i++;
    }
    cout << i;
    return 0;
}