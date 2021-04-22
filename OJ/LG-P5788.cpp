#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node
{
    int num;
    int index;
};
node ans[3000010];
int ans_index = 0;
int A[3000010];
int res[3000010];

int main()
{
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &A[i]);

    ans[ans_index].index = 0;
    ans[ans_index].num = 0x7fffffff;
    for (int i = n; i >= 1; i--)
    {
        while(ans_index && A[i] >= ans[ans_index].num) ans_index--;
        res[i] = ans[ans_index].index;

        ans[++ans_index].num = A[i];
        ans[ans_index].index = i;
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", res[i]);
    return 0;
}