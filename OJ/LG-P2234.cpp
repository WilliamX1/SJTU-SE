#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

struct node
{
    int num;
    int day;
};
node A[35000];
bool myCompare(node a, node b)
{
    return a.num < b.num;
}
int main()
{
    int n; cin >> n;

    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &A[i].num);
        A[i].day = i;
    }

    sort(A + 1, A + n + 1, myCompare);

    long long int m, res = 0;

    for (int i = 1; i <= n; i++)
    {
        if (A[i].day == 1)
            res += A[i].num;
        else
        {
            m = 1e9;
            for (int j = i - 1; j >= 1; j--)
                if (A[j].day < A[i].day)
                {
                    if (A[i].num - A[j].num < m)
                        m = A[i].num - A[j].num;
                    break;
                }
            for (int j = i + 1; j <= n; j++)
                if (A[j].day < A[i].day)
                {
                    if (A[j].num - A[i].num < m)
                        m = A[j].num - A[i].num;
                    break;
                }
            res += m;
        }

    }

    cout << res;
    return 0;
}