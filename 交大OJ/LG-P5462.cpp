#include <bits/stdc++.h>
#include <stdio.h>
#include <stack>
using namespace std;

struct node
{
    int left;
    int right;
};
node A[1000010];
int A_xb = 0;
bool myCompare(node a, node b)
{
    return a.left > b.left;
}
int main()
{
    int n; scanf("%d", &n);
    stack<int> s;

    int tmp;

    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &tmp);
        if (s.empty() || tmp > s.top())
            s.push(tmp);
        else
        {
            A[++A_xb].left = s.top();
            s.pop();
            A[A_xb].right = tmp;
        }
    }

    while(!s.empty())
    {
        A[++A_xb].right = s.top();
        s.pop();
        A[A_xb].left = s.top();
        s.pop();
    }

    sort(A + 1, A + A_xb + 1, myCompare);

    for (int i = 1; i <= A_xb; i++)
        printf("%d %d ", A[i].left, A[i].right);

    return 0;
}