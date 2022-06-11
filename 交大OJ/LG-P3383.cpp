#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
int Prime[100000010];
bool Check[100000010];
int n;
void getPrime()
{
    memset(Check, 0, sizeof(Check));
    int tot = 0;
    for (int i = 2; i <= n; i++)
    {
        if (!Check[i])
            Prime[++tot] = i;
        for (int j = 1; j <= tot; j++)
        {
            if (i * Prime[j] > n)
                break;
            Check[i * Prime[j]] = true;
            if (i % Prime[j] == 0)
                break;
        }
    }
}
int main()
{
    scanf("%d", &n);
    int k; scanf("%d", &k);
    getPrime();

    while(k--)
    {
        scanf("%d", &n);
        printf("%d\n", Prime[n]);
    }

    return 0;
}