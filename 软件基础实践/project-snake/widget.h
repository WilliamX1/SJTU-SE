#ifndef WIDGET_H
#define WIDGET_H

#include "button.h"
#include "map.h"
#include "parameters.h"
#include "record.h"
#include "snake.h"

#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include <QSound>
#include <QWidget>
#include <windows.h>

namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void init(){};
    void saveFile(); //存档
    void readFile(); //读档
protected:
    void keyPressEvent(QKeyEvent *event); //键盘按下事件
    //void keyReleaseEvent(QKeyEvent *event){}; //键盘松开事件
public slots:
    void startGame(bool isRestart = true); //重新开始游戏还是读档
    void pauseGame();   //暂停游戏
    void snakeWalkSlots();  //循环更新地图函数
    void intoSave();    //进入存档模式
    void intoRead();    //进入读档模式
    void normalBeanChange(); //进入食物改变模式
    void lifeBeanChange();
    void speedBeanChange();
    void wallBeanChange();
    void intoAI();
    void is_up_down_wall();
    void is_left_right_wall();
    void is_doublePlayer();
private:
    Ui::Widget *ui;
    QSpinBox* normalBean;
    QSpinBox* lifeBean; //生命值食物
    QSpinBox* speedBean; //速度食物
    QSpinBox* wallBean; //墙体状态食物
    QCheckBox* AI;  //AI点击框
    QCheckBox* up_down_wall;    //上下墙点击框
    QCheckBox* left_right_wall; //左右墙点击框
    QCheckBox* doublePlayer;    //双人模式点击框
    void updateBox();    //更新窗口和点击框状态
    void setControl();  //各种按键/点击框初始化
    void setIsEdit(bool flag);    //设置所有按键/点击框是否可以编辑
};
#endif // WIDGET_H
