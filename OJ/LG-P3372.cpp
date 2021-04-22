#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

const int MAXN = 100010;
long long ans[MAXN << 2]; //存储线段树专用
long long tag[MAXN << 2]; //懒标记数组
inline int ls(int p){return p << 1;}  //左二子是2i
inline int rs(int p){return p << 1 | 1;} //右儿子2i+1
int A[MAXN];

void push_up_sum(int p)
{
    ans[p] = ans[ls(p)] + ans[rs(p)];
} //向上不断维护区间操作
void push_up_min(int p)
{
    ans[p] = min(ans[ls(p)], ans[ls(p)]);
}//维护最小值
void build_tree(int p, int L, int R)
{
    if (L == R) {ans[p] = A[L]; return;};
    //如果左右区间相同，那么必然是叶子结点
    int mid = (L + R) >> 1;
    build_tree(ls(p), L, mid);
    build_tree(rs(p), mid + 1, R);
    push_up_sum(p);
} //建树
inline void f(int p, int L, int R, int k)
{
    tag[p] += (long long)k;
    ans[p] += (long long)k * (R - L + 1);
} //区间统一改变，所以ans数组要加元素个数次
inline void push_down(int p, int L, int R)
{
    int mid = (L + R) >> 1;
    f(ls(p), L, mid, tag[p]);
    f(rs(p), mid + 1, R, tag[p]);
    tag[p] = 0;
    //每次更新两个儿子结点，并以此不断向下传递
}
inline void update(int nL, int nR, int L, int R, int p, int k)
{
    //nL/nR为要修改的区间
    //l/r/p为当前节点所存储的区间以及结点的编号
    if (nL <= L && R <= nR)
    {
        ans[p] += (long long)k * (R - L + 1);
        tag[p] += (long long)k;
        return;
    }
    push_down(p, L, R);

    int mid = (L + R) >> 1;
    if (nL <= mid) update(nL, nR, L, mid, ls(p), k);
    if (nR > mid) update(nL, nR, mid + 1, R, rs(p), k);
    push_up_sum(p);
}
long long query(int nL, int nR, int L, int R, int p)
{
    long long res = 0;
    if (nL <= L && R <= nR) return ans[p];
    int mid = (L + R) >> 1;
    push_down(p, L, R);
    if (nL <= mid) res += query(nL, nR, L, mid, ls(p));
    if (nR > mid) res += query(nL, nR, mid + 1, R, rs(p));
    return res;
}

int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    build_tree(1, 1, n);

    int ch, x, y, k;
    while(m--)
    {
        scanf("%d", &ch);
        if (ch == 1)
        {
            scanf("%d%d%d", &x, &y, &k);
            update(x, y, 1, n, 1, k);
        }
        else if (ch == 2)
        {
            scanf("%d%d", &x, &y);
            printf("%lld\n", query(x, y, 1, n, 1));
        }
    }
    return 0;
}