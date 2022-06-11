#include <bits/stdc++.h>

using namespace std;

struct node
{
    int Start;
    int End;
};
node A[1000010];

bool myCompare(node a, node b)
{
    if (a.End == b.End) return a.Start > b.Start;
    return a.End < b.End;
}
int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i].Start >> A[i].End;
    sort(A + 1, A + n + 1, myCompare);

    int E = 0;
    int sum = 0;
    for (int i = 1; i <= n; i++)
        if (A[i].Start >= E)
        {
            sum++;
            E = A[i].End;
        }
    cout << sum;

    return 0;
}