#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int Con[100010];  //国籍
int Time[100010];  //到达时间
int People[100010][100];
int sum = 0;

int main()
{
    memset(Con, 0, sizeof(Con));

    int n, k; scanf("%d", &n);
    int xb = 1;

    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &Time[i]);
        scanf("%d", &k);
        People[i][0] = k;

        for (int j = 1; j <= k; j++)
        {
            scanf("%d", &People[i][j]);
            if (Con[People[i][j]] == 0) sum++;
            Con[People[i][j]]++;
        }

        while (Time[i] - Time[xb] >= 86400)
        {
            for (int j = 1; j <= People[xb][0]; j++)
            {
                Con[People[xb][j]]--;
                if (Con[People[xb][j]] == 0) sum--;
            }
            xb++;
        }
        printf("%d\n", sum);
    }

    return 0;
}