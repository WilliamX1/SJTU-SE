#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int dp[100010][18];
int ciFang[18];
inline int read()
{
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}
int main()
{
    ciFang[0] = 1;
    for (int i = 1; i < 18; i++)
        ciFang[i] = 2 * ciFang[i - 1];

    int N, M; scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++)
        dp[i][0] = read();
    for (int j = 1; j <= 18; j++)
        for (int i = 1; i <= N - ciFang[j - 1] + 1; i++)
            dp[i][j] = max(dp[i][j - 1], dp[i + ciFang[j - 1]][j - 1]);

    int L, R, p;
    while(M--)
    {
        L = read(); R = read();
        p = (int)(log(R - L + 1) / log(2));
        printf("%d\n", max(dp[L][p], dp[R - ciFang[p] + 1][p]));
    }
    return 0;
}