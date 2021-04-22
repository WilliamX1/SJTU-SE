#include <bits/stdc++.h>

using namespace std;

int n;

int A[30];  //A[i]记录i行的y值
int sum = 0;

bool B[60];

void Dfs(int x)
{
    if (x > n)
    {
        sum++;
        if (sum <= 3)
        {
            for (int i = 1; i <= n; i++)
                cout << A[i] << " ";
            cout << endl;
        }
        return;
    }

    bool flag;
    for (int j = 1; j <= n; j++)
    {
        flag = true;
        for (int i = 1; i < x; i++)
            if (A[i] == j || abs(i - x) == abs(A[i] - j))
            { flag = false; break;}

        if (flag)
        {
            A[x] = j;
            Dfs(x + 1);
        }
    }
    return;
}

int main()
{
    cin >> n;
    memset(A, 0, sizeof(A));
    memset(B, 0, sizeof(B));
    Dfs(1);
    cout << sum;
    return 0;
}