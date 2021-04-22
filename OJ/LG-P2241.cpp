#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n, m; cin >> n >> m; //n × m方格
    int MIN = min(n, m);

    long long int Z = 0; //正方形
    for (int i = 1; i <= MIN; i++)  //正方形边长
        Z += (long long)(m - i + 1) * (long long)(n - i + 1);

    long long int C = 0; //长方形（含正方形）
    for (int i = 1; i <= n; i++)  //长度
        for (int j = 1; j <= m; j++)  //宽度
            C += (long long)(n - i + 1) * (long long)(m - j + 1);
    cout << Z << " " << C - Z;
    return 0;
}