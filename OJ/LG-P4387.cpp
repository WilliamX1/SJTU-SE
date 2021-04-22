#include <bits/stdc++.h>
#include <stdio.h>
#include <stack>

using namespace std;

int in[100010];
int out[100010];

int main()
{
    int t; scanf("%d", &t);
    while(t--)
    {
        int n; scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &in[i]);
        for (int i = 1; i <= n; i++)
            scanf("%d", &out[i]);

        stack<int> s;
        int xb = 1;
        for (int i = 1; i <= n; i++)
        {
            s.push(in[i]);
            while(!s.empty() && s.top() == out[xb])
            {
                s.pop();
                xb++;
            }
        }

        if (s.empty()) printf("Yes\n");
        else printf("No\n");
    }
    return 0;
}