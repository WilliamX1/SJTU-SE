#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int ans[500010];
int dx[500010];  //²î·ÖÊý×é
int res[500010];
int N, M;

int lowbit(int x)
{
    return x & -x;
}
void update(int x, int k)
{
    while(x <= N)
    {
        res[x] += k;
        x += lowbit(x);
    }
    return;
}
void update(int x, int y, int k)
{
    update(x, k);
    update(y + 1, -k);
}
long long getnum(int x)
{
    long long tmp_res = 0;
    while(x >= 1)
    {
        tmp_res += res[x];
        x -= lowbit(x);
    }
    return tmp_res;
}
int main()
{
    scanf("%d%d", &N, &M);

    memset(res, 0, sizeof(res));
    ans[0] = 0;
    for (int i = 1; i <= N; i++)
        scanf("%d", &ans[i]);
    for (int i = 1; i <= N; i++)
    {
        dx[i] = ans[i] - ans[i - 1];
        update(i, dx[i]);
    }
    int ch, x, y, k;
    while(M--)
    {
        scanf("%d", &ch);
        if (ch == 1)
        {
            scanf("%d%d%d", &x, &y, &k);
            update(x, y, k);
        }
        else if (ch == 2)
        {
            scanf("%d", &x);
            printf("%lld\n", getnum(x));
        }
    }

    return 0;
}