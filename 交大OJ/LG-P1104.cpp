#include <bits/stdc++.h>

using namespace std;

struct node
{
    string s;
    int year;
    int month;
    int day;
    int label;
};
node A[128];

bool myCompare(node a, node b)
{
    if (a.year == b.year)
        if (a.month == b.month)
            if (a.day == b.day)
                return a.label > b.label;
            else
                return a.day < b.day;
        else
            return a.month < b.month;
    else
        return a.year < b.year;
}
int main()
{
    int n; cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> A[i].s >> A[i].year >> A[i].month >> A[i].day;
        A[i].label = i;
    }
    sort(A, A + n, myCompare);
    for (int i = 0; i < n; i++)
        cout << A[i].s << endl;
    return 0;
}