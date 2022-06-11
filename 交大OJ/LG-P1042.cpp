#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;

int main()
{
    string s;
    char S[800000];
    int xb = 0;

    int len;
    bool flag = true;
    while(flag)
    {
        cin >> s;
        len = s.length();
        for (int i = 0; i < len; i++)
            if (s[i] != 'E') S[++xb] = s[i];
            else
            {
                flag = false;
                break;
            }
    }

    int x = 0, y = 0;
    for (int i = 1; i <= xb; i++)
    {
        if (S[i] == 'W') x++;
        else if (S[i] == 'L') y++;
        if ((x >= 11 || y >= 11) && abs(x - y) >= 2)
        {
            printf("%d%c%d\n", x, ':', y);
            x = y = 0;
        }
    }
    printf("%d%c%d\n", x, ':', y);
    x = y = 0;

    printf("\n");

    for (int i = 1; i <= xb; i++)
    {
        if (S[i] == 'W') x++;
        else if (S[i] == 'L') y++;
        if ((x >= 21 || y >= 21) && abs(x - y) >= 2)
        {
            printf("%d%c%d\n", x, ':', y);
            x = y = 0;
        }
    }
    printf("%d%c%d", x, ':', y);
    x = y = 0;


    return 0;
}