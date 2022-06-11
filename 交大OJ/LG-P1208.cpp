#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

struct node
{
    int per;
    int weight;
};
node A[5010];
bool myCompare(node a, node b)
{
    return a.per < b.per;
}
int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &A[i].per, &A[i].weight);

    sort(A + 1, A + m + 1, myCompare);

    int res = 0;
    for (int i = 1; i <= m; i++)
    {
        if (A[i].weight > n)
        {
            res += n * A[i].per;
            break;
        }
        else
        {
            res += A[i].weight * A[i].per;
            n -= A[i].weight;
        }
    }
    printf("%d", res);

    return 0;
}