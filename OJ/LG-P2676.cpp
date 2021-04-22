#include <bits/stdc++.h>

using namespace std;

int A[20010];

int main()
{
    long long int N, B; cin >> N >> B;
    for (int i = 1; i <= N; i++)
        cin >> A[i];

    sort(A + 1, A + N + 1);

    long long int sum = 0;
    for (int i = N; i >= 1; i--)
    {
        if (sum >= B) { cout << N - i; break;}
        else sum += (long long int) A[i];
    }

    return 0;
}