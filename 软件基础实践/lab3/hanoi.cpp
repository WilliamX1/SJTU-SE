#include "termio.h"

// Do not delete the following line
char Termio::buffer[Termio::CANVAS_HEIGHT][Termio::CANVAS_WIDTH + 1];

void write_hi() {
    for (int i = 0; i < 5; i++) {
        Termio::buffer[i][0] = '|';
        Termio::buffer[i][5] = '|';
        Termio::buffer[i][9] = '|';
        Termio::buffer[i][13] = '*';
    }
    Termio::buffer[3][13] = ' ';
    Termio::buffer[1][9] = ' ';
    for (int i = 1; i < 5; i++) {
        Termio::buffer[2][i] = '-';
    }
}

int main() {

    // ---- Example: how to use termio.h ----
    // clear screen
    Termio::Clear();
    // reset buffer
    Termio::ResetBuffer();
    write_hi();
    Termio::Draw();
    // ----------- End of example -----------
    // Your code here

    // some output hints that you should use.
    std::cout << "How many disks do you want? (1 ~ 5)" << std::endl;
    std::cout << "Move a disk. Format: x y" << std::endl;
    std::cout << "Auto moving:1->2" << std::endl;
    std::cout << "Congratulations! You win!" << std::endl;

    return 0;
}
