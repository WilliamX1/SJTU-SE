#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
long long int A[2000010];

int main()
{
    long long int n; scanf("%lld", &n);

    long long int a, b, A_xb = 0;
    while(n--)
    {
        scanf("%lld%lld", &a, &b);
        b = a ^ b;
        A[++A_xb] = a;
        if (b != a) A[++A_xb] = b;
    }

    sort(A + 1, A + A_xb + 1);

    long long int M = 1, tmp = 1, num = A[A_xb];

    for (int i = A_xb - 1; i >= 1; i--)
    {
        if (A[i] == A[i + 1])
            tmp++;  //临时数量
        else if (tmp >= M)
        {
            M = tmp;  //最多出现次数
            tmp = 1;  //重新置零
            num = A[i + 1];
        }
        else tmp = 1;
    }

    printf("%lld", num);
    return 0;
}