#include <iostream>

using namespace std;

string mid, last;

void get(int m, int left, int right)
{
    if (left > right) return;

    for (int i = left; i <= right; i++)
        if (mid[i] == last[m])
        {
            cout << last[m];
            get(m - 1 - (right - i), left, i - 1); 
             
            get(m - 1, i + 1, right);
            break;
        }
    return;
}
int main()
{
    cin >> mid >> last;

    get(last.length() - 1, 0, mid.length() - 1);
    
    return 0;
}