#include <iostream>

using namespace std;

int A[10010];
int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i];

    int sum = 0;
    for (int i = 2; i <= n; i++)
        for (int j = i - 1; j >= 1; j--)
            if (A[j] > A[i]) sum++;
    cout << sum;
    return 0;
}