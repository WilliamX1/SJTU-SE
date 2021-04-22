#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int N = 1000000 + 10;
int ans[N];
int ans_index = 0;
void put_in(int x)
{
    ans[++ans_index] = x;  //放入队尾

    int cur = ans_index;
    int tmp = 0;
    while(cur > 1 && ans[cur >> 1] > x)  //父节点比他大
    {
        //父节点与子节点进行交换
        tmp = ans[cur];
        ans[cur] = ans[cur >> 1];
        ans[cur >> 1] = tmp;
        cur = cur >> 1;
    }
    return;
}
void put_out()
{
    ans[1] = ans[ans_index--];  //将最后一个节点放入头结点

    int cur = 1;

    int next, tmp;
    while(cur <= ans_index)
    {
        if ((cur << 1) <= ans_index && (cur << 1 | 1) <= ans_index)  //左右子节点都存在
        {
            next = ans[cur << 1] < ans[cur << 1 | 1] ? cur << 1 : (cur << 1 | 1);
            if (ans[cur] > ans[next])
            {
                tmp = ans[cur];
                ans[cur] = ans[next];
                ans[next] = tmp;
            }else break;
        }
        else if ((cur << 1) <= ans_index)
        {
            next = cur << 1;
            if (ans[cur] > ans[next])
            {
                tmp = ans[cur];
                ans[cur] = ans[next];
                ans[next] = tmp;
            }else break;
        }
        else
            break;

        cur = next;
    }
    return;
}
int main()
{
    int n; scanf("%d", &n);

    int op, x;
    while(n--)
    {
        scanf("%d", &op);
        if (op == 1)
        {
            scanf("%d", &x);
            put_in(x);
        }
        else if (op == 2)
            printf("%d\n", ans[1]);
        else if (op == 3)
            put_out();
    }
    return 0;
}