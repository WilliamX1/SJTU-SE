#include <bits/stdc++.h>

using namespace std;

struct node
{
    string s;
    int label;
};
node A[25];
bool myCompare(node a, node b)
{
    int len_A = a.s.length();
    int len_B = b.s.length();

    if (len_A == len_B)
        for (int i = 0; i < len_A; i++)
            if (a.s[i] != b.s[i]) return a.s[i] > b.s[i];

    return len_A > len_B;
}
int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> A[i].s;
        A[i].label = i;
    }

    sort(A + 1, A + n + 1, myCompare);

    cout << A[1].label << endl << A[1].s;

    return 0;
}
