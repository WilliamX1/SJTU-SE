#include <bits/stdc++.h>

using namespace std;

struct node
{
    int Chinese;
    int Math;
    int English;
    int total;
    int label;
};
node A[310];

bool myCompare(node a, node b)
{
    if (a.total == b.total)
        if (a.Chinese == b.Chinese)
            return a.label < b.label;
        else return a.Chinese > b.Chinese;
    else return a.total > b.total;

}

int main()
{
    int n; cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> A[i].Chinese >> A[i].Math >> A[i].English;
        A[i].total = A[i].Chinese + A[i].Math + A[i].English;
        A[i].label = i + 1;
    }
    for (int i = 0; i < 5; i++)
    {
        nth_element(A, A + i, A + n, myCompare);
        cout << A[i].label << " " << A[i].total << endl;
    }
    return 0;
}