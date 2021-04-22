#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int mod = 100000000 - 3;
struct node
{
    int height;
    int index;
};
node A[100010];
node B[100010];
bool myCompare(node a, node b){return a.height < b.height;};

int ans[100010];
int tmp[100010];
long long res = 0;

void msort(int L, int R)
{
    if (L >= R) return;
    int mid = (L + R) >> 1;
    msort(L, mid);
    msort(mid + 1, R);

    int index = 0, left = L, right = mid + 1;
    while(left <= mid && right <= R)
        if (ans[left] <= ans[right])
            tmp[++index] = ans[left++];
        else
        {
            res = (res + mid + 1 - left) % mod;
            tmp[++index] = ans[right++];
        }
    while(left <= mid) tmp[++index] = ans[left++];
    while(right <= R) tmp[++index] = ans[right++];

    for (int i = 1; i <= index; i++)
        ans[L + i - 1] = tmp[i];
    return;
}
int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &A[i].height);
        A[i].index = i;
    }
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &B[i].height);
        B[i].index = i;
    }

    sort(A + 1, A + n + 1, myCompare);
    sort(B + 1, B + n + 1, myCompare);

    for (int i = 1; i <= n; i++)
        ans[A[i].index] = B[i].index;

    msort(1, n);
    printf("%lld", res);

    return 0;
}