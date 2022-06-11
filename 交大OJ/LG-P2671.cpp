#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
const int N = 100000;
const long long Mod = 10007;
struct node
{
    long long int number;
    long long int color;
    long long int index;
};
node A[N + 10];
bool myCompare(node a, node b)
{
    if (a.color == b.color)
        return a.index & 1;
    return a.color < b.color;
}
int main()
{
    int n, m; scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &A[i].number);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &A[i].color);
    for (int i = 1; i <= n; i++)
        A[i].index = i;
    sort(A + 1, A + n + 1, myCompare);

    int Start = 1, Mid = 1, End = 1;
    long long int res = 0;

    while(Start <= n)
    {
        //奇数序列为【Start，Mid】
        Mid = Start;
        while(Mid <= n && A[Mid].color == A[Start].color && (A[Mid].index & 1)) Mid++;
        Mid--;

        //偶数序列为【Mid + 1，End】
        End = Mid + 1;
        while(End <= n && A[End].color == A[Start].color && !(A[End].index & 1)) End++;
        End--;

        int tmp_sum = 0;
        int tmp_num = Mid - Start - 1;

        for (int i = Start; i <= Mid; i++) tmp_sum += A[i].number;
        for (int i = Start; i <= Mid; i++)
        {
            res += A[i].index * (tmp_sum + A[i].number * tmp_num);
            res %= Mod;
        }

        tmp_sum = 0;
        tmp_num = End - (Mid + 1) - 1;
        for (int i = Mid + 1; i <= End; i++) tmp_sum += A[i].number;
        for (int i = Mid + 1; i <= End; i++)
        {
            res += A[i].index * (tmp_sum + A[i].number * tmp_num);
            res %= Mod;
        }
        End = Mid = Start = End + 1;
    }
    cout << res;

    return 0;
}