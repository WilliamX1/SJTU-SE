#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    string str;
    cin >> str;
    for (int i = str.length() - 1; i >= 0; i--)
        cout << str[i];
    return 0;
}