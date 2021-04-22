#include <bits/stdc++.h>
#include <queue>
using namespace std;

int main()
{
    priority_queue< int, vector<int>, greater<int> > q;

    int n; scanf("%d", &n);
    int tmp;
    while(n--)
    {
        scanf("%d", &tmp);
        q.push(tmp);
    }

    int sum = 0;
    int x1, x2, x;

    if (n == 1) cout << q.top();
    else
        while(1)
        {
            x1 = q.top();  q.pop();
            x2 = q.top();  q.pop();

            x = x1 + x2;
            sum += x;

            if (q.empty()) break;
            q.push(x);
        }

    cout << sum;
    return 0;
}