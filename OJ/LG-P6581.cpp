#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
    long long int n; scanf("%lld", &n);
    unsigned long long int res;

    if (n <= 0) { printf("%d", 0); return 0; }
    else
    {
        res = n;
        printf("%llu", 2 * res - 1);
    }
    return 0;
}