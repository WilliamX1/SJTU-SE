#include <iostream>
#include <stdio.h>
using namespace std;

struct node
{
    double D;  //距离出发点距离
    double P;  //油价
};

int main()
{
    double D1, C, D2, P; cin >> D1 >> C >> D2 >> P;
    int N; cin >> N;
    node A[20];

    A[0].D = 0; A[0].P = P; //出发点
    A[N + 1].D = D1;
    A[N + 1].P = 0;  //终点

    for (int i = 1; i <= N; i++)
    {
        cin >> A[i].D >> A[i].P;
        if (A[i].D - A[i - 1].D > C * D2){ cout << "No Solution"; return 0;}
    }
    if (A[N + 1].D - A[N].D > C * D2) {cout << "No Solution"; return 0;}

    double sum = 0, oil = 0;
    for (int i = 0; i <= N; i++)
    {
        for (int j = i + 1; j <= N + 1; j++)
            if (A[j].D - A[i].D > C * D2)
            {
                sum += A[i].P * C;
                oil += C - (A[j - 1].D - A[i].D) / D2;
                i = j - 2;
                break;
            }
            else if (A[j].P <= A[i].P)
            {
                sum += A[i].P * ((A[j].D - A[i].D) / D2 - oil);
                oil = 0;
                i = j - 1;
                break;
            }
    }
    printf("%.2f", sum);
    return 0;
}