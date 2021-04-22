#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
bool A[1010];
int isT[7] = {0, 1, 3, 7, 30, 120, 365};
int ciFang[12] = {0,1,2,4,8,16,32,64,128,256,512,1012};

int main()
{
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        scanf("%d", &A[i]);

    int ses = 0;
    int ses_less = 0;
    int res = 0;
    for (int i = 1; i <= N; i++)
    {
        int index = i;

        ses = max(ses - ses_less, 0);
        while(index <= N && A[index])
        {
            ses++;
            int j = 1;
            while(j <= 6 && ses >= isT[j]) j++;
            j--;

            res += j;
            index++;
        }

        ses_less = 0;
        while(index <= N && !A[index])
        {
            ses_less++;
            index++;
        }
        if (ses_less > 11)
            ses_less = 11;
        ses_less = ciFang[ses_less];

        i = index - 1;
    }
    cout << res;
    return 0;
}