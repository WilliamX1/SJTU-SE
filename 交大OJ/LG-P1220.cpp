#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node
{
    int pos;
    int P;
    bool flag;
    node():flag(false){};
};
node A[52];
int n, c, res = 0x7fffffff;
void Dfs(int index, int x, int ans, int time, int left, int right)
{
    //index是灯号数组下标，x是当前已关灯数量，ans代表耗电数，time代表已过的时间
    if (ans >= res) return;
    if (x == n)
        res = min(res, ans);

    A[index].flag = true;

    //int left = index, right = index;
    //while(left >= 1 && A[left].flag) left--;
    //while(right <= n && A[right].flag) right++;

    if (left >= 1)
        Dfs(left, x + 1, ans + A[left].P * (time + A[index].pos - A[left].pos), time + A[index].pos - A[left].pos, left - 1, right);
    if (right <= n)
        Dfs(right, x + 1, ans + A[right].P * (time + A[right].pos - A[index].pos), time + A[right].pos - A[index].pos, left, right + 1);

    A[index].flag = false;

    return;
}
int main()
{
    scanf("%d%d", &n, &c);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &A[i].pos, &A[i].P);

    Dfs(c, 1, 0, 0, c - 1, c + 1);
    printf("%d", res);
    return 0;
}