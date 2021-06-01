#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string GetPalindromeString(string s) {
    int n = s.size();
    vector<int> fail(n, -1);
    for (int i = 1; i < n; i++) {
        int j = fail[i - 1];
        while (j != -1 && s[j + 1] != s[i]) j = fail[j];
        if (s[j + 1] == s[i]) fail[i] = j + 1;
    };
    int best = -1;
    for (int i = n - 1; i >= 0; i--) {
        while (best != -1 && s[best + 1] != s[i]) best = fail[best];
        if (s[best + 1] == s[i]) ++best;
    };
    string add = (best == n - 1? "" : s.substr(best + 1, n));
    reverse(add.begin(), add.end());
    return add + s;
}

int main()
{
    GetPalindromeString("123454321124");
    return 0;
}

// 1 2 3 4 5 4 3 2 1 6 7 8
// 8 7 6 1 2 3 4 5 4 3 2 1