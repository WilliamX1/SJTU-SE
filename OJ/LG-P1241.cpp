#include <bits/stdc++.h>

using namespace std;

int T[120];  //0代表待补全，1代表已匹配

int main()
{
    string s; cin >> s;
    int len = s.length();

    memset(T, 0, sizeof(T));

    for (int i = 0; i < len; i++)
        if (s[i] == ']')
        {
            for (int j = i - 1; j >= 0; j--)
                if (T[j] == 0)
                    if (s[j] == '(') break;
                    else if (s[j] == '[') { T[i] = T[j] = 1; break;}

        }
        else if (s[i] == ')')
        {
            for (int j = i - 1; j >= 0; j--)
                if (T[j] == 0)
                    if (s[j] == '[') break;
                    else if (s[j] == '(') { T[i] = T[j] = 1; break;}
        }

    for (int i = 0; i < len; i++)
        if (T[i] == 0)
        {
            if (s[i] == '(' || s[i] == ')') printf("()");
            else if (s[i] == '[' || s[i] == ']') printf("[]");
        }
        else
            printf("%c", s[i]);

    return 0;
}