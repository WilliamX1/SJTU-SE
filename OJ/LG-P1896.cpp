#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int sit[2000], gs[2000];
int sit_index = 0;
int N, K;

long long dp[10][2000][100]; //第i行用第j个状态还剩k个国王

void Dfs(int he, int sum, int node)
{
    if (node >= N)
    {
        sit[++sit_index] = he;
        gs[sit_index] = sum;
        return;
    }

    Dfs(he, sum, node + 1); //不用第node个
    Dfs(he + (1 << node), sum + 1, node + 2); //用第node个，此时node要加2，及跳过下一个格子
}
int main()
{
    scanf("%d%d", &N, &K);
    Dfs(0, 0, 0);

    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= sit_index; i++) dp[1][i][K - gs[i]] = 1;

    for (int i = 2; i <= N; i++)
        for (int j = 1; j <= sit_index; j++)
            for (int k = 1; k <= sit_index; k++)
            {

                if (sit[j] & sit[k]) continue; //上下有
                if ((sit[j] << 1) & sit[k]) continue;  //最好左移，右移前段会补1出问题
                if (sit[j] & (sit[k] << 1)) continue;
                //cout << sit[j] << " " << sit[k] << "Yes" << endl;

                for (int s = K; s >= gs[j]; s--) dp[i][j][s - gs[j]] += dp[i - 1][k][s];
            }
    long long res = 0;
    for (int i = 1; i <= sit_index; i++) res += dp[N][i][0];

    //for (int i = 1; i <= sit_index; i++)
        //cout << hex << sit[i] << endl;
    printf("%lld", res);
    return 0;
}