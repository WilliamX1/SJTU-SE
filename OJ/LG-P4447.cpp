#include <bits/stdc++.h>
#include <stdio.h>
#include <queue>
using namespace std;

int A[100010];

int main()
{
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);
    sort(A + 1, A + N + 1);

    int q_index = 0;
    queue<int> q[1000];

    int tmp_index, tmp_min;
    q[++q_index].push(A[1]);

    for (int i = 2; i <= N; i++)
    {
        tmp_index = 0;
        tmp_min = 1000000;  //无限大
        for (int j = 1; j <= q_index; j++)
            if (q[j].back() == A[i] - 1 && q[j].size() < tmp_min)
            {
                tmp_min = q[j].size();
                tmp_index = j;
            }
        if (tmp_index)
            q[tmp_index].push(A[i]);
        else q[++q_index].push(A[i]);
    }

    int res = 1000000;
    for (int i = 1; i <= q_index; i++)
        if (q[i].size() < res) res = q[i].size();
    cout << res;
    return 0;
}