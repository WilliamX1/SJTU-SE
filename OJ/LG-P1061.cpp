#include <iostream>

using namespace std;

int main()
{
    int s, t, w; cin >> s >> t >> w;

    string str; cin >> str;

    bool flag = true;;
    for (int i = 1; i <= 5 && flag; i++)
    {
        flag = false;
        for (int j = w - 1; j >= 0; j--)
        {
            if (str[j] - 'a' + 1 == t - (w - 1 - j))
                continue;
            else {
                flag = true;
                str[j]++;
                for (int k = j + 1; k <= w - 1; k++)
                    str[k] = str[k - 1] + 1;
                break;
            }
        }
        if (flag) cout << str << endl;
    }
    return 0;
}