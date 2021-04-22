#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n, m; cin >> n >> m;
    int *A = new int[m];
    for (int i = 0; i < m; i++)
        cin >> A[i];
    sort(A, A + m);
    for (int i = 0; i < m; i++)
        cout << A[i] << " ";
    return 0;
}