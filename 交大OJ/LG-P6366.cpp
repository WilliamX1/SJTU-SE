#include <bits/stdc++.h>

using namespace std;

int A[10000000];
int A_xb = 0;

int B[10000000];

void f (string s, int x)
{
    //打表换算16进制到二进制
    char ch = s[x];
    if (ch == 'A')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == 'B')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == 'C')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == 'D')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == 'E')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == 'F')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '0')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '1')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == '2')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == '3')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '4')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '5')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == '6')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == '7')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '8')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '9')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1;}
    return;
}

int main()
{
    string s; cin >> s;
    int len = s.length();

    for (int i = 0; i < len; i++)
    {
         char ch = s[i];
    if (ch == 'A')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == 'B')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == 'C')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == 'D')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == 'E')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == 'F')
    { A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '0')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '1')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == '2')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == '3')
    { A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '4')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '5')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 1;}
    else if (ch == '6')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 0;}
    else if (ch == '7')
    { A[++A_xb] = 0; A[++A_xb] = 1; A[++A_xb] = 1; A[++A_xb] = 1;}
    else if (ch == '8')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 0;}
    else if (ch == '9')
    { A[++A_xb] = 1; A[++A_xb] = 0; A[++A_xb] = 0; A[++A_xb] = 1;}
    }

    int start = 1;
    while(A[start] == 0 && start <= 4) start++;  //去除前导0

    int res = 1000000000;

    B[start - 1] = 0, B[start] = 0;  //第一张牌不翻转
    for (int i = start + 1; i <= 4 * len + 1; i++)
        B[i] = (B[i - 1] + B[i - 2] + A[i - 1]) % 2;
    if (!B[4 * len + 1])
        res = min(res, accumulate(B + start, B + 4 * len + 1, 0));


    B[start - 1] = 0, B[start] = 1;  //第一张牌要翻转
    for (int i = start + 1; i <= 4 * len + 1; i++)
        B[i] = (B[i - 1] + B[i - 2] + A[i - 1]) % 2;
    if (!B[4 * len + 1])
        res = min(res, accumulate(B + start, B + 4 * len + 1, 0));

    if (res == 1000000000)
        cout << "No";
    else
        cout << res;

    return 0;
}