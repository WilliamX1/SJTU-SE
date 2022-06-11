#ifndef RECORD_H
#define RECORD_H

#include <QWidget>

#include "map.h"
#include "snake.h"
#include "widget.h"

class record
{
public:
    record(){};
    bool flag = false; // 是否有记录
    int userNumber = 0;  // 玩家数量
    int Boy_Score = 0, Girl_Score = 0;  // 玩家分数
    class map* copyMap = NULL;
    class snake* copyBoy = NULL;
    class snake* copyGirl = NULL;
    void copySave(QWidget *parent); // 保存至idx中
    void readSave(QWidget *parent, int idx); // 读取存档
};

#endif // RECORD_H
