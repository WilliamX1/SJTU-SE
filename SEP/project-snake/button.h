#ifndef BUTTON_H
#define BUTTON_H


#include "parameters.h"
#include "widget.h"

#include <QKeyEvent>
#include <QPushButton>

class button
{
public:
    QPushButton* start = NULL;  //开始按键
    QPushButton* save = NULL;   //存档按键
    QPushButton* read = NULL;   //读档按键
    QPushButton* pause = NULL;  //暂停按键
    button(){};
    void init(QWidget *parent = nullptr);
};

#endif // BUTTON_H
