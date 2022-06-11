#include <bits/stdc++.h>

using namespace std;

int A[10010];

int main()
{
    int N, M; cin >> N >> M;
    for (int i = 0; i < N; i++) cin >> A[i];
    while(M--) next_permutation(A, A + N);
    for (int i = 0; i < N - 1; i++)
        cout << A[i] << " ";
    cout << A[N - 1];
    return 0;
}