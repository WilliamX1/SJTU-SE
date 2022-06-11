#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int A[500010];
int tmp[500010];
long long res = 0;
void msort(int left, int right)
{
    if (left >= right) return;

    int mid = (left + right) >> 1;
    msort(left, mid);
    msort(mid + 1, right);

    int L = left, R = mid + 1, index = 0;
    while(L <= mid && R <= right)
        if (A[L] > A[R])
        {
            tmp[++index] = A[R];
            res += mid + 1 - L;
            R++;
        }
        else
            tmp[++index] = A[L++];
    while(L <= mid)
        tmp[++index] = A[L++];
    while(R <= right)
        tmp[++index] = A[R++];
    //复制原数组
    for (int i = 1; i <= index; i++)
        A[left + i - 1] = tmp[i];
    return;
}
int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);
    msort(1, n);
    printf("%lld", res);
    return 0;
}
