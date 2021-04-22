#include <iostream>

using namespace std;

int main()
{
    double A, B, C; cin >> A >> B >> C;
    bool flag = false;

    for (int a = 1; a <= 9; a++)
        for (int b = 1; b <= 9; b++)
            if (b == a) continue;
            else
                for (int c = 1; c <= 9; c++)
                    if (c == b || c == a) continue;
                    else
                        for (int d = 1; d <= 9; d++)
                            if (d == c || d == b || d == a) continue;
                            else
                                for (int e = 1; e <= 9; e++)
                                    if (e == d || e == c || e == b || e == a) continue;
                                    else
                                        for (int f = 1; f <= 9; f++)
                                            if (f == e || f == d || f == c || f == b || f == a) continue;
                                            else
                                                for (int g = 1; g <= 9; g++)
                                                    if (g == f || g == e || g == d || g == c || g == b || g == a) continue;
                                                    else
                                                        for (int h = 1; h <= 9; h++)
                                                            if (h == g || h == f || h == e || h == d || h == c || h == b || h == a) continue;
                                                            else
                                                                for (int i = 1; i <= 9; i++)
                                                                    if (i == h || i == g || i == f || i == e || i == d || i == c || i == b || i == a) continue;
                                                                    else
                                                                    {
                                                                        double x1 = a * 100 + b * 10 + c * 1;
                                                                        double x2 = d * 100 + e * 10 + f * 1;
                                                                        double x3 = g * 100 + h * 10 + i * 1;
                                                                        if (x1 * B == x2 * A && x1 * C == x3 * A)
                                                                        {
                                                                            cout << x1 << " " << x2 << " " << x3 << endl;
                                                                            flag = true;
                                                                        }
                                                                    }
    if (!flag) cout << "No!!!";
    return 0;
}