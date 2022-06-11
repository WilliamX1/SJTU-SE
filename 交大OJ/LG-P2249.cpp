#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int A[1000010];

int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);

    int x;
    while(m--)
    {
        scanf("%d", &x);
        int left = 1, right = n, mid;
        while(right >= left)
        {
            mid = (right + left) >> 1;
            if (A[mid] == x) {right = mid; break;}
            else if (A[mid] < x) left = mid + 1;
            else right = mid - 1;
        }

        if (A[right] == x)
        {
            left = 1;
            while(right >= left)
            {
                mid = (left + right) >> 1;
                if (A[mid] == x) right = mid - 1;
                else left = mid + 1;
            }

            while(A[++right] != x);
            printf("%d ", right);
        }
        else printf("-1 ");
    }
    return 0;
}