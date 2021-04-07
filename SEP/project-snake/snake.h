#ifndef SNAKE_H
#define SNAKE_H

#include "button.h"
#include "map.h"
#include "parameters.h"
#include "widget.h"

#include <QEventLoop>
#include <QLabel>
#include <QTextStream>

#include <algorithm>
#include <ctime>
#include <deque>
#include <qmath.h>
#include <queue>
#include <stdlib.h>
#include <synchapi.h>

class snake
{
private:
    struct snakeNode
    {
        int x, y;
        QLabel* label = NULL;
        snakeNode(int xx = 0, int yy = 0, QWidget *parent = nullptr)
            :x(xx), y(yy){
            label = new QLabel(parent);
        }
        snakeNode(snakeNode* copy, QWidget *parent)
            :x(copy->x), y(copy->y){
            label = new QLabel(parent);
            label->setGeometry(x * (Parameters.gridLength + Parameters.gridGap), y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
            label->hide();
        }
    };
    std::deque<snakeNode*> SnakeBody;
    int AIdown = 0; //计算AI蛇移动
    int AIright = 0;
public:
    int score = 0;  // 玩家分数
    int life = 3; // 生命值
    int Dir = 1; // 1,2,3,4代表上下左右
    QLabel* Score = NULL;
    QLabel* Life = NULL;

    snake(){};
    void init(QWidget *parent = nullptr, bool isBoy = 1);
    void move(QWidget *parent = nullptr, int Dir = 1, int isBoy = 1);   //改变方向
    void walk(QWidget *parent = nullptr, int isBoy = 1);    //蛇移动
    void drawSnake(QWidget *parent = nullptr, bool isBoy = 1);  //画蛇身
    bool getFruit(QWidget *parent = nullptr, int isBoy = 1); // 判断是否吃到果实
    static bool failed(QWidget *parent = nullptr, int isBoy = 1);   //判输

    void copy(QWidget *parent, snake &res_Snake, bool isBoy);

    void updateScore(QWidget* parent, bool isBoy);  //更新玩家分数
    void clearSnake(QWidget* parent);   //清理蛇身残余颜色
    void saveSnake(QWidget* parent, QTextStream& In);   //存档蛇身
    void regainSnake(QWidget* parent, QTextStream& In); //读取蛇身bool

    bool getAIroad(QWidget *parent);    //获取下一个AI路
    bool AIcrash(QWidget* parent, int x, int y);    //判断AI蛇是否会撞蛇/撞墙
};

#endif // SNAKE_H
