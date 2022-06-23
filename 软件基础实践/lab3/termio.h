#pragma once
/* some code from Libconio 1.0 (C) 2004 FSL A.C. */

#ifdef _WIN32
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void clrscr()
{
    system("cls");
}
#else
#include <termios.h>
#include <stdio.h>
#define ESC 033 /* Escape char */

static int getch()
{
    struct termios t;
    int c;

    tcgetattr(0, &t);
    t.c_lflag &= ~ECHO + ~ICANON;
    tcsetattr(0, TCSANOW, &t);
    fflush(stdout);
    c = getchar();
    t.c_lflag |= ICANON + ECHO;
    tcsetattr(0, TCSANOW, &t);
    return c;
}

static void clrscr()
{
    printf("%c[2J", ESC);
    printf("%c[%d;%dH", ESC, 0, 0);
}
#endif

#include <iostream>
#include <cstring>
using namespace std;

class Termio
{
private:
    int tot_num;
    struct stack
    {
        int *array;
        int tot;
        stack(){
            array = new int[10];
            tot = 0;
        }
        void push(int tar){
            array[tot++] = tar;
        }
        int pop(){
            if (tot){
                tot--;
                return array[tot];
            }
            else return -1;
        }
        int top(){
            return array[tot - 1];
        }
        bool empty(){
            return !tot;
        }
    };
    static void drawLine(int rod, int row, int tot)
    {
        int colume = 0;
        switch(rod){
           case 1: colume = 5; break;
           case 2: colume = 20; break;
           case 3: colume = 35; break;
           default: return;
        }

        tot = (tot + 1) >> 1;
        while(tot--)
             buffer[row][colume + tot] = buffer[row][colume - tot] = '*';
        return;
    }
public:  // 0-base
    // donot change the width and height

    stack* disk;
    stack pre_command;
    static const int CANVAS_WIDTH = 41;
    static const int CANVAS_HEIGHT = 11;
    static char buffer[CANVAS_HEIGHT][CANVAS_WIDTH + 1];

    char GetChar() {
        return getch();
    }

    void Clear() {
        clrscr();
    }

    void Draw() {
        memset(buffer, ' ', sizeof(buffer));
        for (int i = 0; i < CANVAS_WIDTH; i++)
            buffer[CANVAS_HEIGHT - 1][i] = '-';
        for (int i = 0; i < CANVAS_HEIGHT; i++)
            buffer[i][5] = buffer[i][20] = buffer[i][35] = '|';

        /*for (int i = 1; i <= 3; i++)
            for (int j = 0; j < disk[i].tot; j++)
                cout << disk[i].array[j] << " ";*/

        for (int i = 1; i <= 3; i++)
            for (int j = 0; j < disk[i].tot; j++)
                drawLine(i, 9 - 2 * j, disk[i].array[j]);

        // print to screen
        for (int i = 0; i < CANVAS_HEIGHT; i++)
        {
            for (int j = 0; j < CANVAS_WIDTH; j++)
                printf("%c", buffer[i][j]);
            printf("\n");
        }
        return;
    }

    void initial(int num){
        tot_num = num;
        disk = new stack[4];

        for (int i = 1; i <= num; i++)
            disk[1].push((num << 1) + 3 - (i << 1));
        return;
    }

    bool move(int from, int to, bool flag = true){
        if (disk[from].empty()) return false;
        if (!disk[to].empty() && disk[from].top() > disk[to].top()) return false;

        if (flag) pre_command.push(from), pre_command.push(to);
        disk[to].push(disk[from].pop());
        return true;
    }

    void hanoi(int n, int A, int B, int C){
        if (n == 0) return;
        hanoi(n - 1, A, C, B);
        std::cout << "Auto moving:" << A << "->" << B << std::endl;
        move(A, B, false); Draw();
        hanoi(n - 1, C, B, A);
    }
    void autoMove(){
        Clear();
        while(!pre_command.empty()){
            int from = pre_command.pop(), to = pre_command.pop();
            std::cout << "Auto moving:" << from << "->" << to << std::endl;
            move(from, to, false);
            Draw();
        }
        hanoi(tot_num, 1, 2, 3);
        return;
    }
    bool over(){
        return disk[1].empty() && disk[3].empty();
    }
};
