#include "button.h"

button Button;

void button::init(QWidget *parent){

    if (start || save || pause) return;

    //设置Button颜色
    start = new QPushButton("(Re)Start", parent);
    save = new QPushButton("Save", parent);
    read = new QPushButton("Read", parent);
    pause = new QPushButton("Pause/GoOn", parent);
    start->setStyleSheet("QPushButton{ background-color: rgb(143,122,102); border-radius: 10px; font: bold; color: white; }");
    pause->setStyleSheet("QPushButton{ background-color: rgb(193,82,102); border-radius: 10px; font: bold; color: white; }");
    save->setStyleSheet("QPushButton{ background-color: rgb(100,22,102); border-radius: 10px; font: bold; color: white; }");
    read->setStyleSheet("QPushButton{ background-color: rgb(100,22,102); border-radius: 10px; font: bold; color: white; }");

    QFont fontBtn("Consolas",15);
    start->setFont(fontBtn);    //设置字体
    start->setGeometry(740, 400, 200, 80);  //设置位置和长宽
    start->show();
    start->setEnabled(true);

    pause->setFont(fontBtn);
    pause->setGeometry(740, 500, 200, 80);
    pause->show();
    pause->setEnabled(false);

    save->setFont(fontBtn);
    save->setGeometry(740, 600, 100, 80);
    save->show();
    save->setEnabled(false);

    read->setFont(fontBtn);
    read->setGeometry(840, 600, 100, 80);
    read->show();
    read->setEnabled(false);
    return;
}
