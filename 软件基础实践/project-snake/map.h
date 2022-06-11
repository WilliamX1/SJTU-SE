#ifndef MAP_H
#define MAP_H

#include "parameters.h"
#include "record.h"
#include "snake.h"
#include "widget.h"

#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QTextStream>
#include <QTimer>
#include <ctime>
#include <deque>
#include <stdlib.h>

class map
{
    friend class widget;
    friend class snake;
private:
public:
    struct mapNode{
        int x, y;   //坐标
        int length;
        int Type; //0代表边界，1代表空位，2代表蛇，3代表果实，4代表加生命
        QLabel* label = NULL;
        mapNode(int x = 0, int y = 0, int length = Parameters.gridLength, QWidget *parent = nullptr)
            :x(x), y(y), length(length), Type(1){
            label = new QLabel(parent);
        };
        mapNode(mapNode* copy, QWidget *parent)
            :x(copy->x), y(copy->y), length(copy->length), Type(copy->Type){
            label = new QLabel(parent);
            label->setGeometry(x * (Parameters.gridLength + Parameters.gridGap), y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
            label->hide();
        }
    };
    QLabel* Title = NULL; // 贪吃蛇大作战标题框
    QLabel* Final = NULL; // 结束
    std::deque<map::mapNode*> Food;
public:
    mapNode* Grid[16][16];
    QTimer timer;
    int moveSpeed = 500;
    int normalBean = 6; //生成6个普通加分食物
    int lifeBean = 1; //生成1个增加生命值食物
    int speedBean = 2; //生成2个增加/减少速度食物
    int wallBean = 3; //生成3个改变墙体状态食物
    bool up_down_Wall = true;   //是否有上下墙
    bool left_right_Wall = true;    //是否有左右墙

    map();
    void init(QWidget *parent);
    void drawMap(QWidget *parent, bool LR = false, bool UD = false);    //画地图
    void createFruit(QWidget *parent, int Type, int num = 1);   //生成食物
    void copy(QWidget *parent, map &res_Map);

    void saveMap(QWidget* parent, QTextStream& In); //存档地图
    void regainMap(QWidget* parent, QTextStream& In);   //读档地图
};

#endif // MAP_H
