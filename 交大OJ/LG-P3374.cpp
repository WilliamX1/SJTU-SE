#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int ans[500010];
long long res[500010];
int n, m;

int lowbit(int x)
{
    return x & -x;
}
void update(int x, int k)
{
    while(x <= n)
    {
        res[x] += k;
        x += lowbit(x);
    }
}
long long getsum(int x)
{
    long long tmp_res = 0;
    while(x > 0)
    {
        tmp_res += res[x];
        x -= lowbit(x);
    }
    return tmp_res;
}

int main()
{
    scanf("%d%d", &n, &m);
    memset(ans, 0, sizeof(ans));
    memset(res, 0, sizeof(res));
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &ans[i]);
        update(i, ans[i]);
    }
    int ch, x, y, k;
    while(m--)
    {
        scanf("%d", &ch);
        if (ch == 1)
        {
            scanf("%d%d", &x, &k);
            update(x, k);
        }
        else if (ch == 2)
        {
            scanf("%d%d", &x, &y);
            printf("%lld\n", getsum(y) - getsum(x - 1));
        }
    }

    return 0;
}