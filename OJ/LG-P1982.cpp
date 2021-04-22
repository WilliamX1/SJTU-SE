#include <bits/stdc++.h>

using namespace std;
typedef __int128 ll;
ll A[1000010];  //数字
ll B[1000010];  //特征值
ll dp[1000010];  //
ll score[1000010];  //分数
inline int read()
{
    int x;
    scanf("%d", &x);
    return x;
}
inline void print(__int128 x){
    if(x < 0){
        putchar('-');
        x = -x;
    }
    if(x > 9)
        print(x / 10);
    putchar(x % 10 + '0');
}

int main()
{
    ll n, p;
    n = read(); p = read();
    for (int i = 1; i <= n; i++)
        A[i] = read();

    ll M = B[1] = dp[1] = A[1];
    for (int i = 2; i <= n; i++)
    {
        if (dp[i - 1] > 0)
            dp[i] = dp[i - 1] + A[i];
        else
            dp[i] = A[i];

        if (dp[i] > M) M = dp[i];
        B[i] = M;
    }

    score[1] = B[1];
    M = B[1] + score[1];
    for (int i = 2; i <= n; i++)
    {
        if (B[i - 1] + score[i - 1] > M) M = B[i - 1] + score[i - 1];
        score[i] = M;
    }

    print(*max_element(score + 1, score + n + 1) % p);

    return 0;
}
