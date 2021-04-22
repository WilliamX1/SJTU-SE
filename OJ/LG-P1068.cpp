#include <bits/stdc++.h>

using namespace std;

struct node
{
    int k;  //报名号
    int s;  //面试成绩
};
node A[5010];

bool myCompare(node a, node b)
{
    if (a.s == b.s)
        return a.k < b.k;
    return a.s > b.s;
}
int main()
{
    int n, m; cin >> n >> m;
    m *= 1.5;

    int k, s;
    for (int i = 1; i <= n; i++)
    {
        scanf("%d%d", &A[i].k, &A[i].s);
    }

    sort(A + 1, A + n + 1, myCompare);


    for (int i = m + 1; i <= n; i++)
        if (A[i].s == A[m].s) m++;

    cout << A[m].s << " " << m << endl;
    for (int i = 1; i <= m; i++)
        cout << A[i].k << " " << A[i].s << endl;

    return 0;
}