#include <bits/stdc++.h>
#include <stdio.h>
#include <queue>
using namespace std;

struct node
{
    int layer;
    int num;
    node(int l, int n):layer(l), num(n){};
};
int A[300];
bool T[300];

int main()
{
    int n; scanf("%d", &n);
    int a, b; scanf("%d%d", &a, &b);

    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    memset(T, 0, sizeof(T));

    queue<node> q;
    q.push(node(a, 0));

    int tmp_num, tmp_layer , res = -1;

    while(!q.empty())
    {
        tmp_num = q.front().num;
        tmp_layer = q.front().layer;
        if (tmp_layer == b) { res = tmp_num; break;};

        q.pop();

        if (T[tmp_layer]) continue;
        T[tmp_layer] = true;

        if (tmp_layer - A[tmp_layer] >= 1) q.push(node(tmp_layer - A[tmp_layer], tmp_num + 1));
        if (tmp_layer + A[tmp_layer] <= n) q.push(node(tmp_layer + A[tmp_layer], tmp_num + 1));
    }

    printf("%d", res);

    return 0;
}