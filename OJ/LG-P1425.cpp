#include <iostream>
using namespace std;

int main()
{
	int h1, s1, h2, s2;cin >> h1 >> s1 >> h2 >> s2;
	if (s2 >= s1)
	    cout << (h2 - h1) << " " << s2 - s1;
	else
	    cout << (h2 - h1 - 1) << " " << s2 + 60 - s1;
	return 0;
}