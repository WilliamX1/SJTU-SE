#include <iostream>
#include <queue>

using namespace std;

int main()
{
    int k, N; cin >> k >> N;
    queue<int> q;

    while(N > 0){
        q.push(N & 1);
        N >>= 1;
    }

    long long int res = 0;
    long long tmp = 1;
    while(!q.empty()){
        res += tmp * q.front();
        q.pop();
        tmp *= k;
    }
    cout << res;

    return 0;
}