#include "termio.h"

// Do not delete the following line
char Termio::buffer[Termio::CANVAS_HEIGHT][Termio::CANVAS_WIDTH + 1];

int main() {

    // ---- Example: how to use termio.h ----
    // clear screen
    //Termio::Clear();
    // reset buffer
    //Termio::ResetBuffer();
    //write_hi();
    //Termio::Draw();
    // ----------- End of example -----------
    // Your code here

    // some output hints that you should use.

    cout << "./hanoi\n";

    Termio myGame;
    flag1:;
    std::cout << "How many disks do you want? (1 ~ 5)" << std::endl;
    string get; cin >> get;
    int num = get[0] - '0';
    if (get.length() != 1) goto flag1;
    else if (num == 'q' - '0' || num == 'Q' - '0') return 0;
    else if (num < 1 || num > 5) goto flag1;
    else{
        //Termio::Clear();
        myGame.initial(num);
    }

    bool flag = false;
    flag2:myGame.Clear(), myGame.Draw();
    std::cout << "Move a disk. Format: x y" << std::endl;
    string getx, gety; cin >> getx >> gety;
    int numx = getx[0] - '0', numy = gety[0] - '0';
    if (getx.length() != 1 || gety.length() != 1) goto flag2;
    else if (!numx && !numy){
        flag = true;
        myGame.autoMove();
    }
    else if (numx < 1 || numx > 3 || numy < 1 || numy > 3) goto flag2;
    else if (!myGame.move(numx, numy) || !myGame.over()) goto flag2;

    if (myGame.over()){
        if (!flag) {
            myGame.Clear();
            myGame.Draw();
        }
        std::cout << "Congratulations! You win!" << std::endl;
        goto flag1;
    }

    return 0;
}
