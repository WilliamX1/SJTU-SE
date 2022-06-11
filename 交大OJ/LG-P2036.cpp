#include <bits/stdc++.h>

using namespace std;

struct node
{
    long long int s;  //酸度
    long long int k;  //苦度
};
node A[12];


int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i].s >> A[i].k;

    long long int t_s = 1;  //总酸度
    long long int t_k = 0;  //总苦度
    long long int x, MIN = 1e9;
    for (long long int i = 1; i < pow(2.0, double(n)); i++)
    {
        t_s = 1; t_k = 0;

        x = i;
        for (int j = 1; j <= n; j++)
        {
            if (x % 2 == 1)
            {
                t_s *= A[j].s; if (t_s > 1e10) { t_s = 1e18; break;}
                t_k += A[j].k;
            }
            x = x >> 1;
        }

        if (abs(t_s - t_k) < MIN) MIN = abs(t_s - t_k);
    }
    cout << MIN;
    return 0;
}