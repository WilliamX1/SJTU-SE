#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

struct node
{
    double m;  //重量
    double v;  //价格
};
node A[110];
bool myCompare(node a, node b)
{
    return a.v / a.m > b.v / b.m;
}
int main()
{
    int N, T; cin >> N >> T;
    for (int i = 0; i < N; i++)
        cin >> A[i].m >> A[i].v;
    sort(A, A + N, myCompare);

    double sum = 0;
    for (int i = 0; i < N; i++)
        if (T <= A[i].m)
        {
            sum += double(T) * A[i].v / A[i].m;
            break;
        }
        else
        {
            sum += A[i].v;
            T -= A[i].m;
        }
    printf("%.2f", sum);
    return 0;
}