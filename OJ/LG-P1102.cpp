#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
struct node
{
    int num;
    long long times;
    node():times(1){};
};
node A[200010];
int ans[200010];

int main()
{
    int N, C; scanf("%d%d", &N, &C);
    C = abs(C);  //绝对值化处理

    for (int i = 1; i <= N; i++)
        scanf("%d", &ans[i]);
    sort(ans + 1, ans + N + 1);

    int index = 0;

    int j;
    for (int i = 1; i <= N; i = j)
    {
        A[++index].num = ans[i];
        j = i + 1;
        while(j <= N && ans[j] == ans[i]) j++;

        A[index].times = j - i;
    }

    long long res = 0;
    if (C == 0)
    {
        for (int i = 1; i <= index; i++)
            res += A[i].times * (A[i].times - 1) / 2;
        printf("%lld", res);
        return 0;
    }

    int left = 1, right = 1;
    while(right <= index)
    {
        while(right <= index && A[right].num - A[left].num < C) right++;
        if (A[right].num - A[left].num == C) { res += A[left].times * A[right].times; left++;}
        while(left <= index && A[right].num - A[left].num > C) left++;
        if (A[right].num - A[left].num == C) { res += A[left].times * A[right].times; right++;}
    }
    printf("%lld", res);
    return 0;
}