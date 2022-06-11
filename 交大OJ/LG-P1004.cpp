#include <iostream>
#include <stdio.h>

using namespace std;
struct node
{
    int x1, x2, y1, y2;
};

node A[10010];

int main()
{
    int n; scanf("%d", &n);

    int len_x, len_y;
    for (int i = 1; i <= n; i++)
    {

        scanf("%d%d%d%d", &A[i].x1, &A[i].y1, &len_x, &len_y);
        A[i].x2 = A[i].x1 + len_x;
        A[i].y2 = A[i].y1 + len_y;
    }

    int x, y; scanf("%d%d", &x, &y);

    int i = n;
    while(i >= 1)
    {
        if (A[i].x1 <= x && x <= A[i].x2 && A[i].y1 <= y && y <= A[i].y2)
        {
            printf("%d", i);
            break;
        }
        i--;
    }
    if (i == 0) printf("%d", -1);

    return 0;
}