#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int Landmine[21];
int A[21][21];
bool Go[21];
int MAX = 0;
int N;

int dp[21];
int tmp_Road[21];
int Road[21];
int _index;

void Dfs(int x, int label, int num)
{
    if (num <= dp[x]) return;  //如果挖到这没以前大
    tmp_Road[label] = x;

    bool flag = true;
    for (int i = 1; i <= N; i++)
        if (A[x][i] && !Go[x])
        {
            Go[x] = true;
            Dfs(i, label + 1, num + Landmine[i]);
            Go[x] = false;
            flag = false;
        }
    if (flag && num > MAX)
    {
        MAX = num;
        for (int i = 1; i <= label; i++)
            Road[i] = tmp_Road[i];
        _index = label;
    }
}
int main()
{
    scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &Landmine[i]);
    for (int i = 1; i <= N; i++)
        for (int j = i + 1; j <= N; j++)
            scanf("%d", &A[i][j]);



    for (int i = 1; i <= N; i++)
    {
        memset(Go, 0, sizeof(Go));
        memset(dp, 0, sizeof(dp));

        Dfs(i, 1, Landmine[i]);
    }
    for (int i = 1; i <= _index; i++)
        cout << Road[i] << " ";
    cout << endl << MAX;
    return 0;
}