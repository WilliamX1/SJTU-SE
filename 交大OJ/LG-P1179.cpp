#include <iostream>

using namespace std;

int main()
{
    int L, R; cin >> L >> R;
    int sum = 0;
    for (int i = L; i <= R; i++)
    {
        int num = i;
        while(num > 0)
        {
            if (num % 10 == 2) sum++;
            num /= 10;
        }
    }
    cout << sum;
    return 0;
}
