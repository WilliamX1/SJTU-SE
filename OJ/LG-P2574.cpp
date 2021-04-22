#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int MAXN = 200000 + 10;
long long ans[MAXN << 2];
bool tag[MAXN << 2];
inline int ls(int p){return p << 1;};
inline int rs(int p){return p << 1 | 1;};
int A[MAXN];

void push_up_sum(int p)
{
    ans[p] = ans[ls(p)] + ans[rs(p)];
}
void build_tree(int p, int L, int R)
{
    if (L == R) {ans[p] = A[L]; return;};

    int mid = (L + R) >> 1;
    build_tree(ls(p), L, mid);
    build_tree(rs(p), mid + 1, R);
    push_up_sum(p);
}
inline void f(int p, int L, int R)
{
    ans[p] = R - L + 1 - ans[p];
    tag[p] = !tag[p];
}
inline void push_down(int p, int L, int R)
{
    if (!tag[p]) return;

    int mid = (L + R) >> 1;
    f(ls(p), L, mid);
    f(rs(p), mid + 1, R);
    tag[p] = false;
}
inline void update(int nL, int nR, int L, int R, int p)
{
    if (nL <= L && R <= nR)
    {
        ans[p] = R - L + 1 - ans[p];
        tag[p] = !tag[p];
        return;
    }
    push_down(p, L, R);

    int mid = (L + R) >> 1;
    if (nL <= mid) update(nL, nR, L, mid, ls(p));
    if (nR > mid) update(nL, nR, mid + 1, R, rs(p));
    push_up_sum(p);
}
long long query(int nL, int nR, int L, int R, int p)
{
    long long res = 0;
    if (nL <= L && R <= nR) return ans[p];
    push_down(p, L, R);

    int mid = (L + R) >> 1;
    if (nL <= mid) res += query(nL, nR, L, mid, ls(p));
    if (nR > mid) res += query(nL, nR, mid + 1, R, rs(p));
    return res;
}
int main()
{
    int n, m; scanf("%d%d", &n, &m);

    memset(tag, 0, sizeof(tag));
    memset(ans, 0, sizeof(ans));

    string str; cin >> str;
    for (int i = 1; i <= n; i++)
        A[i] = str[i - 1] - '0';
    build_tree(1, 1, n);

    int op, l, r;
    while(m--)
    {
        scanf("%d%d%d", &op, &l, &r);
        if (op)
            printf("%lld\n", query(l, r, 1, n, 1));
        else if (!op)
            update(l, r, 1, n, 1);
    }

    return 0;
}