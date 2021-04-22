#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int N = 10000 + 10;
int n, m;
int Head[N];

int Find(int x)
{
    int y = x;
    while(Head[y] != y)
        y = Head[y];

    int tmp;
    while(Head[x] != x)
    {
        tmp = Head[x];
        Head[x] = y;
        x = tmp;
    }
    return x;
}
int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        Head[i] = i;
    int Zi, Xi, Yi;
    while(m--)
    {
        scanf("%d%d%d", &Zi, &Xi, &Yi);
        if (Zi == 1)
        {
            int X_head = Find(Xi);
            int Y_head = Find(Yi);
            Head[Y_head] = X_head;
        }
        else if (Zi == 2)
        {
            if (Find(Xi) == Find(Yi))
                printf("Y\n");
            else printf("N\n");
        }
    }
    return 0;
}