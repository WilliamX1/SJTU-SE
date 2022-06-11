#include <iostream>
#include <stdio.h>

using namespace std;

struct node
{
    int on;
    int off;
    int total;
};

node A[20];

int main()
{
    int a, n, m, x; scanf("%d%d%d%d", &a, &n, &m, &x);

    A[1].on = a, A[1].off = 0, A[1].total = a;

    for (int i = 0; i <= 10000; i++)
    {
        A[2].on = A[2].off = i;
        A[2].total = A[1].total;

        for (int j = 3; j < n; j++)
        {
            A[j].on = A[j - 1].on + A[j - 2].on;
            A[j].off = A[j - 1].on;
            A[j].total = A[j - 1].total + A[j].on - A[j].off;
        }

        if (A[n - 1].total == m)
        {
            printf("%d", A[x].total);
            break;
        }
    }
    return 0;
}