#include <bits/stdc++.h>

using namespace std;

int main()
{
    double H, S1, V, L, K;
    int n;
    cin >> H >> S1 >> V >> L >> K >> n;

    double t1, t2;
    t1 = sqrt(H / 5);
    t2 = sqrt((H - K) / 5);

    cout << min(int (S1 - t2 * V + L), n) - max(0, int(S1 - t1 * V));

    return 0;
}