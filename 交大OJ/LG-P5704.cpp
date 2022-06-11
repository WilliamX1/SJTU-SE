#include <iostream>
using namespace std;

int main()
{
    char ch;
    cin >> ch;
    cout << char(ch - 'a' + 'A');
    return 0;
}