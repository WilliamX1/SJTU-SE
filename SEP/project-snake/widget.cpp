#include "widget.h"
#include "ui_widget.h"
#include "QGraphicsOpacityEffect"
#include "QLineEdit"

#include <QMessageBox>

extern map Map;
extern button Button;
extern snake Boy, Girl;
extern record Record[];

volatile int userNumber = 1;  //玩家数量：1 or 2
volatile bool isAI = false; //智能蛇
bool isSave = false;  //是否进入存档编辑状态
bool isRead = false;  //是否进入读档编辑状态

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    setFocusPolicy(Qt::StrongFocus);  //点击后、 按键盘后接受键盘

    //界面设置
    this->setFixedSize(Parameters.widgetWidth, Parameters.widgetHeight);
    this->setWindowTitle("贪吃蛇大冒险");
    Map.init(this);
    Boy.init(this, 1);
    Girl.init(this, 0);
    Button.init(this);

    Map.drawMap(this);
    //Boy.drawSnake(this, 1);

    //timer与更新相关联
    QObject::connect(&Map.timer, SIGNAL(timeout()), this, SLOT(snakeWalkSlots()));
    //各种Button与相关功能相关联
    QObject::connect(Button.start, SIGNAL(clicked()), this, SLOT(startGame()));
    QObject::connect(Button.pause, SIGNAL(clicked()), this, SLOT(pauseGame()));
    QObject::connect(Button.save, SIGNAL(clicked()), this, SLOT(intoSave()));
    QObject::connect(Button.read, SIGNAL(clicked()), this, SLOT(intoRead()));

    //各种Button和CheckBox初始化
    setControl();

    Button.start->setEnabled(true);
    Button.pause->setEnabled(false);
    Button.read->setEnabled(true);
    Button.save->setEnabled(true);

    //设置背景图片
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    QPixmap background = QPixmap("C:\\Users\\14475\\Desktop\\Software_practice\\PRO\\files\\start_2.jpg").scaled(QSize(Parameters.widgetWidth, Parameters.widgetHeight));
    if (background.isDetached()) {
        pal.setBrush(backgroundRole(), background);
        setPalette(pal);
    }
    //设置背景音乐
    QSound *music = new QSound("C:\\Users\\14475\\Desktop\\Software_practice\\PRO\\files\\music.wav", this);
    music->play();
    music->setLoops(100);

    return;
}
void Widget::startGame(bool isRestart)
{
    //豆子不可编辑
    setIsEdit(false);

    Parameters.isEditUserNumber = false;
    Parameters.isEditWall = false;
    Parameters.isEditBean = false;

    if (isRestart){ //如果不是读档，则初始化
        Map.init(this);
        Boy.init(this, true);
        Girl.init(this, false);
        if (userNumber == 1) {
            Girl.clearSnake(this);
        }
        Button.init(this);
    }
    //画图
    Map.drawMap(this);
    Boy.drawSnake(this, 1);
    if (userNumber == 2) Girl.drawSnake(this, 0);
    Boy.updateScore(this, true);
    if (userNumber == 2) Girl.updateScore(this, false);

    //开始移动
    Map.timer.start(Map.moveSpeed);

    //设置按钮不可用，以便窗口获取焦点
    Button.start->setEnabled(false);
    Button.save->setEnabled(false);
    Button.read->setEnabled(false);
    Button.pause->setEnabled(true);
};
void Widget::pauseGame()
{
    //如果游戏时点击则暂停，如果暂停时点击则继续

    if (Map.timer.isActive())
    {
        Parameters.isEditWall = true;
        Parameters.isEditBean = true;
        Parameters.isEditUserNumber = true;

        Map.timer.stop();
        Button.start->setEnabled(true);
        Button.save->setEnabled(true);
        Button.read->setEnabled(true);

        //设置可以编辑
        setIsEdit(true);
    }
    else {
        Parameters.isEditWall = false;
        Parameters.isEditBean = false;
        Parameters.isEditUserNumber = false;

        Map.timer.start(Map.moveSpeed);
        Button.start->setEnabled(true);
        Button.save->setEnabled(false);
        Button.read->setEnabled(false);

        //设置不可编辑
        setIsEdit(false);
    }
    return;
};
void Widget::snakeWalkSlots()
{
    //更新计时器
    Map.timer.start(Map.moveSpeed);

    //更新分数板
    updateBox();

    //更新蛇行走
    if (isAI) { //AI蛇自动获取下一步行走
        Boy.getAIroad(this);
        if (userNumber == 2) Girl.getAIroad(this);
    }
    Boy.walk(this, 1);
    if (userNumber == 2) Girl.walk(this, 0);

    //重绘蛇
    Boy.drawSnake(this, 1);
    if (userNumber == 2) Girl.drawSnake(this, 0);

    //地图更新
    Map.drawMap(this);

    return;
}
void Widget::intoSave()
{
    //进入存档编辑模式
    isSave = true;
    saveFile();
    return;
}
void Widget::intoRead()
{
    //进入读档编辑模式
    isRead = true;
    readFile();
    return;
}
void Widget::normalBeanChange()
{
    //-1是角，0是墙，1是地，2是蛇，3是果实，4是生命值，5是加速/减速，6是改变墙体
    Map.createFruit(this, 3, normalBean->value() - Map.normalBean);
    Map.normalBean = normalBean->value();
    Map.drawMap(this);
    return;
}
void Widget::lifeBeanChange()
{
    //-1是角，0是墙，1是地，2是蛇，3是果实，4是生命值，5是加速/减速，6是改变墙体
    Map.createFruit(this, 4, lifeBean->value() - Map.lifeBean);
    Map.lifeBean = lifeBean->value();
    Map.drawMap(this);
    return;
}
void Widget::speedBeanChange()
{
    //-1是角，0是墙，1是地，2是蛇，3是果实，4是生命值，5是加速/减速，6是改变墙体
    Map.createFruit(this, 5, speedBean->value() - Map.speedBean);
    Map.speedBean = speedBean->value();
    Map.drawMap(this);
    return;
}
void Widget::wallBeanChange()
{
    //-1是角，0是墙，1是地，2是蛇，3是果实，4是生命值，5是加速/减速，6是改变墙体
    Map.createFruit(this, 6, wallBean->value() - Map.wallBean);
    Map.wallBean = wallBean->value();
    Map.drawMap(this);
    return;
}
void Widget::intoAI()
{
    if (AI->isChecked()) {
       Map.up_down_Wall = Map.left_right_Wall = false;
       isAI = true;
    } else isAI = false;
    return;
}
void Widget::is_up_down_wall()
{
    Map.drawMap(this, true, false);
    return;
}
void Widget::is_left_right_wall(){
    Map.drawMap(this, false, true);
    return;
}
void Widget::is_doublePlayer(){
    if (doublePlayer->isChecked()) {
        userNumber = 2;
    }
    else userNumber = 1;
    return;
}
void Widget::saveFile(){
    //打开并准备重写文件
    QFile A("RECORD.txt");
    A.open(QIODevice::WriteOnly);
    A.close();

    A.resize(0);
    A.open(QIODevice::WriteOnly);

    if (A.isOpen()){
        QTextStream In(&A); //写入工具
        //写入蛇速
        In << Map.moveSpeed << Qt::endl;
        //写入地图可编辑内容状态
        In << Map.normalBean << Qt::endl;
        In << Map.lifeBean << Qt::endl;
        In << Map.speedBean << Qt::endl;
        In << Map.wallBean << Qt::endl;
        In << Map.up_down_Wall << Qt::endl;
        In << Map.left_right_Wall << Qt::endl;
        //写入男蛇分数和方向
        In << Boy.score << Qt::endl;
        In << Boy.life << Qt::endl;
        In << Boy.Dir << Qt::endl;
        //写入女蛇分数和方向
        In << Girl.score << Qt::endl;
        In << Girl.life << Qt::endl;
        In << Girl.Dir << Qt::endl;
        //写入是否可编辑
        In << userNumber << Qt::endl;
        In << isAI << Qt::endl;
        In << isSave << Qt::endl;
        In << isRead << Qt::endl;
        //写入地图各点状态
        Map.saveMap(this, In);
        //写入蛇身位置
        Boy.saveSnake(this, In);
        if (userNumber == 2) Girl.saveSnake(this, In);
    }

    return;
}
void Widget::readFile(){

    QFile A("RECORD.txt");
    A.open(QIODevice::ReadOnly);

    if (!A.exists()) return;

    //豆子不可编辑
    setIsEdit(false);

    Parameters.isEditUserNumber = false;
    Parameters.isEditWall = false;
    Parameters.isEditBean = false;

    if (A.isOpen()){
        QTextStream In(&A);
        //读取蛇速
        Map.moveSpeed = In.readLine().toInt();
        //读取CheckBox状态
        Map.normalBean = In.readLine().toInt();
        normalBean->setValue(Map.normalBean);
        Map.lifeBean = In.readLine().toInt();
        lifeBean->setValue(Map.lifeBean);
        Map.speedBean = In.readLine().toInt();
        speedBean->setValue(Map.speedBean);
        Map.wallBean = In.readLine().toInt();
        wallBean->setValue(Map.wallBean);
        //读取上下墙状态
        Map.up_down_Wall = In.readLine().toInt();
        up_down_wall->setCheckState(Map.up_down_Wall == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        Map.left_right_Wall = In.readLine().toInt();
        left_right_wall->setCheckState(Map.left_right_Wall == true ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        //读取男蛇分数和方向
        Boy.score = In.readLine().toInt();
        Boy.life = In.readLine().toInt();
        Boy.Dir = In.readLine().toInt();
        //读取女蛇分数和方向
        Girl.score = In.readLine().toInt();
        Girl.life = In.readLine().toInt();
        Girl.Dir = In.readLine().toInt();
        //读取可编辑状态
        userNumber = In.readLine().toInt();
        isAI = In.readLine().toInt();
        isSave = In.readLine().toInt();
        isRead = In.readLine().toInt();
        //读取地图各点状态并更新
        Map.regainMap(this, In);
        Map.drawMap(this);
        //读取男蛇蛇身并更新
        Boy.clearSnake(this);
        Boy.regainSnake(this, In);
        Boy.updateScore(this, true);
        Boy.drawSnake(this, true);

        if (userNumber == 2) {
            //读取女蛇蛇身并更新
            Girl.clearSnake(this);
            Girl.regainSnake(this, In);
            Girl.updateScore(this, false);
            Girl.drawSnake(this, false);
        }
    }
    //设置按钮不可用，以便窗口获取焦点
    Button.start->setEnabled(false);
    Button.save->setEnabled(false);
    Button.read->setEnabled(false);
    Button.pause->setEnabled(true);

    //开始移动
    Map.timer.start(Map.moveSpeed);

    return;
}
void Widget::updateBox(){
    normalBean->setValue(Map.normalBean);
    lifeBean->setValue(Map.lifeBean);
    speedBean->setValue(Map.speedBean);
    wallBean->setValue(Map.wallBean);
    AI->setCheckState(isAI == true? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    up_down_wall->setCheckState(Map.up_down_Wall == true? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    left_right_wall->setCheckState(Map.left_right_Wall == true? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    doublePlayer->setCheckState(userNumber == 2 ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}
void Widget::setControl()
{
    //设置编辑模式
    AI = new QCheckBox(this);
    AI->setGeometry(950, 450, 150, 50);
    AI->setEnabled(true);
    AI->show();
    AI->setText("AI模式");
    QObject::connect(AI, SIGNAL(stateChanged(int)), this, SLOT(intoAI()));

    up_down_wall = new QCheckBox(this);
    up_down_wall->setGeometry(950, 500, 150, 50);
    up_down_wall->setEnabled(true);
    up_down_wall->show();
    up_down_wall->setCheckState(Qt::CheckState::Checked);
    up_down_wall->setText("上下墙体");
    QObject::connect(up_down_wall, SIGNAL(clicked()), this, SLOT(is_up_down_wall()));

    left_right_wall = new QCheckBox(this);
    left_right_wall->setGeometry(950, 550, 150, 50);
    left_right_wall->setEnabled(true);
    left_right_wall->show();
    left_right_wall->setCheckState(Qt::CheckState::Checked);
    left_right_wall->setText("左右墙体");
    QObject::connect(left_right_wall, SIGNAL(clicked()), this, SLOT(is_left_right_wall()));

    doublePlayer = new QCheckBox(this);
    doublePlayer->setGeometry(950, 600, 150, 50);
    doublePlayer->setEnabled(true);
    doublePlayer->show();
    doublePlayer->setText("双人模式");
    QObject::connect(doublePlayer, SIGNAL(stateChanged(int)), this, SLOT(is_doublePlayer()));

    normalBean = new QSpinBox(this);
    normalBean->setGeometry(950, 50, 150, 50);
    normalBean->setEnabled(true);
    normalBean->setRange(0, 20);
    normalBean->show();
    normalBean->value();
    normalBean->setPrefix("普通食物：");
    normalBean->setValue(Map.normalBean);
    QObject::connect(normalBean, SIGNAL(valueChanged(int)), this, SLOT(normalBeanChange()));

    lifeBean = new QSpinBox(this);
    lifeBean->setGeometry(950, 150, 150, 50);
    lifeBean->setEnabled(true);
    lifeBean->setRange(0, 20);
    lifeBean->show();
    lifeBean->value();
    lifeBean->setPrefix("生命食物：");
    lifeBean->setValue(Map.lifeBean);
    QObject::connect(lifeBean, SIGNAL(valueChanged(int)), this, SLOT(lifeBeanChange()));

    speedBean = new QSpinBox(this); //生成2个增加/减少速度食物
    speedBean->setGeometry(950, 250, 150, 50);
    speedBean->setEnabled(true);
    speedBean->setRange(0, 20);
    speedBean->show();
    speedBean->value();
    speedBean->setPrefix("速度食物：");
    speedBean->setValue(Map.speedBean);
    QObject::connect(speedBean, SIGNAL(valueChanged(int)), this, SLOT(speedBeanChange()));

    wallBean = new QSpinBox(this); //生成3个改变墙体状态食物
    wallBean->setGeometry(950, 350, 150, 50);
    wallBean->setEnabled(true);
    wallBean->setRange(0, 20);
    wallBean->show();
    wallBean->value();
    wallBean->setPrefix("翻墙食物：");
    wallBean->setValue(Map.wallBean);
    QObject::connect(wallBean, SIGNAL(valueChanged(int)), this, SLOT(wallBeanChange()));
}
void Widget::setIsEdit(bool flag)
{
    normalBean->setEnabled(flag);
    lifeBean->setEnabled(flag);
    speedBean->setEnabled(flag);
    wallBean->setEnabled(flag);
    AI->setEnabled(flag);
    up_down_wall->setEnabled(flag);
    left_right_wall->setEnabled(flag);
    doublePlayer->setEnabled(flag);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_1: //存档/读档模式下可以存档/读档
        if (isSave){
            Record[0].copySave(this);
            isSave = false;
            break;
        };
        if (isRead && Record[0].flag) {
            Record[0].readSave(this, 0);
            isRead = false;
            startGame(false);
            break;
        }; break;
    case Qt::Key_2: //存档/读档模式下可以存档/读档
        if (isSave) {
            Record[1].copySave(this);
            isSave = false;
            break;
        };
        if (isRead && Record[1].flag) {
            Record[1].readSave(this, 1);
            isRead = false;
            startGame(false);
            break;
        }; break;
    case Qt::Key_3: //存档/读档模式下可以存档/读档
        if (isSave) {
            Record[2].copySave(this);
            isSave = false;
            break;
        }
        if (isRead && Record[2].flag) {
            Record[2].readSave(this, 2);
            isRead = false;
            startGame(false);
            break;
        }; break;
    case Qt::Key_F1: //最开始模式下可以更改玩家数量
        if (Parameters.isEditUserNumber) {
            userNumber = 1;
        }; break;
    case Qt::Key_F2: //最开始模式下可以更改玩家数量
        if (Parameters.isEditUserNumber) {
            userNumber = 2;
        }; break;
    case Qt::Key_F3: //加普通豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 3, 1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F4: //减普通豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 3, -1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F5: //加生命值豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 4, 1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F6: //减生命值豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 4, -1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F9: //加加速豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 5, 1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F10: //减加速豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 5, -1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F11: //加墙体豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 6, 1);
            Map.drawMap(this);
        } break;
    case Qt::Key_F12: //加墙体豆子1颗
        if (Parameters.isEditBean && !isAI) {
            Map.createFruit(this, 6, -1);
            Map.drawMap(this);
        } break;
    case Qt::Key_U:
         if (Parameters.isEditWall) {
            Map.drawMap(this, true, false);
         }
         break;
    case Qt::Key_L:
         if (Parameters.isEditWall) {
            Map.drawMap(this, false, true);
         }
         break;
    case Qt::Key_W:
        Boy.move(this, 1, 1); break;
    case Qt::Key_D:
        Boy.move(this, 2, 1); break;
    case Qt::Key_S:
        Boy.move(this, 3, 1); break;
    case Qt::Key_A:
        Boy.move(this, 4, 1); break;
    case Qt::Key_Up:
        Girl.move(this, 1, 0); break;
    case Qt::Key_Right:
        Girl.move(this, 2, 0); break;
    case Qt::Key_Down:
        Girl.move(this, 3, 0); break;
    case Qt::Key_Left:
        Girl.move(this ,4, 0); break;
    case Qt::Key_I:
        {
            if (!isAI) {
                Map.up_down_Wall = Map.left_right_Wall = false;
                isAI = true;
            } else isAI = false;
        } break;
    }
    return;
}; //键盘按下事件
