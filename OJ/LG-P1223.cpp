#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

struct node
{
    int T;
    int label;
};
node A[1010];
bool myCompare(node a, node b)
{
    return a.T < b.T;
}
int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> A[i].T;
        A[i].label = i;
    }
    sort(A + 1, A + n + 1, myCompare);

    long long int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        cout << A[i].label << " ";
        sum += (long long)A[i].T * (long long)(n - i);
    }
    printf("\n%.2f", double(sum) / n);
    return 0;
}