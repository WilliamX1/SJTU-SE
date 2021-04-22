#include <bits/stdc++.h>

using namespace std;

int A[100010];
int left_sum_less[100010];
int left_sum_more[100010];
int right_sum_less[100010];
int right_sum_more[100010];
int main()
{
    int n, b; cin >> n >> b;
    int xb;
    for (int i = 1; i <= n; i++)
    {
        cin >> A[i];
        if (A[i] == b) xb = i;
        A[i] = A[i] > b ? 1 : -1;
    }
    A[xb] = 0;

    memset(left_sum_less, 0, sizeof(left_sum_less));
    memset(left_sum_more, 0, sizeof(left_sum_more));
    memset(right_sum_less, 0, sizeof(right_sum_less));
    memset(right_sum_more, 0, sizeof(right_sum_more));

    int sum = 0;
    for (int i = xb - 1; i >= 1; i--)
    {
        sum += A[i];
        if (sum >= 0) left_sum_more[sum]++;
        else left_sum_less[-sum]++;
    }

    sum = 0;
    for (int i = xb + 1; i <= n; i++)
    {
        sum += A[i];
        if (sum >= 0) right_sum_more[sum]++;
        else right_sum_less[-sum]++;
    }

    int res = 1;  //本身
    res += left_sum_more[0] * right_sum_more[0] + left_sum_more[0] + right_sum_more[0];
    for (int i = 1; i <= 100000; i++)
        res += left_sum_less[i] * right_sum_more[i] + left_sum_more[i] * right_sum_less[i];
    cout << res;

    return 0;
}