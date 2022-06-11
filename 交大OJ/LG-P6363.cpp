#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

double Team[30][30];
struct node
{
    double score;
    char ch;
};
node A[1024];

bool myCompare(node a, node b)
{
    if (a.score == b.score) return a.ch < b.ch;
    return a.score > b.score;
}

int main()
{
    int n, k; cin >> n >> k;

    //输入各个选手的得分和队伍，记录最大队伍数量
    for (int i = 1; i <= n; i++)
        cin >> A[i].score >> A[i].ch;
    //输入i队对j队的打分

    memset(Team, 0, sizeof(Team));
    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= k; j++)
            cin >> Team[i][j];

    for (int j = 1; j <= k; j++)
    {
        for (int i = 1; i <= k; i++)
            Team[0][j] += Team[i][j];
        Team[0][j] /= k;
        //重新筛选
        int tmp = 0;  //记录被筛选掉的队伍数量
        for (int i = 1; i <= k; i++)
            if (abs(Team[i][j] - Team[0][j]) > 15)
            { Team[i][j] = 0; tmp++;}
        //重新计算平均值
        Team[0][j] = 0.0;
        for (int i = 1; i <= k; i++)
            Team[0][j] += Team[i][j];
        Team[0][j] = round(Team[0][j] / (k - tmp));
        //四舍五入取整

    }

    //计算每位选手最终得分
    for (int i = 1; i <= n; i++)
        A[i].score = round(A[i].score * 0.6 + Team[0][A[i].ch - 'A' + 1] * 0.4);

    //重新排序
    sort(A + 1, A + n + 1, myCompare);

    //按序输出
    for (int i = 1; i <= n; i++)
        cout << A[i].score << " " << A[i].ch << endl;

    return 0;
}
