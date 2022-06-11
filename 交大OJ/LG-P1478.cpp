#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int n, s;
int h;
struct node
{
    int xi;
    int yi;
};
node A[5010];

bool myCompare(node a, node b)
{
    if (a.xi > h) return false;
    if (b.xi > h) return true;

    return a.yi < b.yi;
}
int main()
{
    scanf("%d%d", &n, &s);
    int a, b;
    scanf("%d%d", &a, &b);
    h = a + b;

    for (int i = 1; i <= n; i++)
        scanf("%d%d", &A[i].xi, &A[i].yi);
    sort(A + 1, A + n + 1, myCompare);

    int x = 1;
    while(x <= n && s >= A[x].yi && A[x].xi <= h)
    {
        s -= A[x].yi;
        x++;
    }
    cout << x - 1;

    return 0;
}