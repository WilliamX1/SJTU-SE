#include <bits/stdc++.h>

using namespace std;

int res[300];

int main()
{
    int n; cin >> n;
    memset(res, 0, sizeof(res));

    res[0] = 1;
    for (int i = 0; i <= n; i++)
    {
        int yushu = 0;
        for (int j = 0; j <= 205; j++)
            res[j] *= 2;
        for (int j = 0; j <= 205; j++)
        {
            res[j] += yushu;
            if (res[j] >= 10)
            {
                yushu = res[j] / 10;
                res[j] %= 10;
            }
            else yushu = 0;
        }
    }

    int tmp_index = 0;
    int yushu = 2;
    while(1)
    {
        res[tmp_index] -= yushu;
        if (res[tmp_index] >= 0) break;
        res[tmp_index] += 10;
        yushu = 1;
        tmp_index++;
    }

    int index = 0;
    for (int i = 205; i >= 0; i--)
        if (res[i] != 0)
        {
            index = i;
            break;
        }
    for (int i = index; i >= 0; i--)
        cout << res[i];

    return 0;
}