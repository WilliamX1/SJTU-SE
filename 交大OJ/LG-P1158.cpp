#include <bits/stdc++.h>

using namespace std;

struct node
{
    int A;
    int B;
};
node T[100010];
bool myCompare(node a, node b)
{
    return a.A > b.A;
}
int main()
{
    int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
    int N; cin >> N;
    int tmp1, tmp2;
    for (int i = 1; i <= N; i++)
    {
        cin >> tmp1 >> tmp2;
        T[i].A = (x1 - tmp1) * (x1 - tmp1) + (y1 - tmp2) * (y1 - tmp2);
        T[i].B = (x2 - tmp1) * (x2 - tmp1) + (y2 - tmp2) * (y2 - tmp2);
    }
    sort(T + 1, T + N + 1, myCompare);

    T[0].A = T[0].B = 0;
    int B_max = 0;
    int sum_min = 1000000000;
    for (int i = 1; i <= N; i++)
    {
        B_max = max(B_max, T[i - 1].B);
        sum_min = min(sum_min, T[i].A + B_max);
    }
    cout << sum_min;
    return 0;
}