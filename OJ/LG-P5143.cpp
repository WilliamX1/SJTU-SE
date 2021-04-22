#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

struct node
{
    int x, y, z;
};
node A[50010];

bool myCompare(node a, node b){return a.z < b.z;};

int main()
{
    int n; cin >> n;
    for (int i = 0; i < n; i++)
        cin >> A[i].x >> A[i].y >> A[i].z;
    sort(A, A + n, myCompare);

    double sum = 0;

    for (int i = 1; i < n; i++)
        sum += sqrt((A[i].x - A[i - 1].x) * (A[i].x - A[i - 1].x) + (A[i].y - A[i - 1].y) * (A[i].y - A[i - 1].y) + (A[i].z - A[i - 1].z) * (A[i].z - A[i - 1].z));
    printf("%.3f", sum);
    return 0;
}