#include <iostream>
#include <bits/stdc++.h>

using namespace std;
int A[128];
int B[128];

int main()
{
    int n; cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> A[i];
    for (int i = 1; i <= n; i++)
        cin >> B[i];

    int More = 0, Less = 0, Equal = 0;
    for (int i = 1; i <= n; i++)
        if (A[i] == B[i])
            Equal++;
        else if (A[i] > B[i])
            More++;
        else
            Less++;

    if (More == 0)
        cout << -1;
    else cout << Less / More + 1;

    return 0;
}