#include "snake.h"

#include <QString>

snake Boy, Girl;
extern map Map;
extern button Button;
extern int userNumber;
extern bool isAI;

void snake::init(QWidget *parent, bool isBoy){
    //清空SnakeBody
    if (!SnakeBody.empty()) SnakeBody.clear();

    //计算蛇头坐标
    int X = isBoy ? Parameters.Boy_Start_X : Parameters.Girl_Start_X;
    int Y = isBoy ? Parameters.Boy_Start_Y : Parameters.Girl_Start_Y;

    //计算蛇尾坐标
    int next_X = X, next_Y = Y;
    switch (Dir) {
    case 1: next_Y++; break;
    case 2: next_X--; break;
    case 3: next_Y--; break;
    case 4: next_X++; break;
    }
    //创建蛇头蛇尾节点
    snakeNode* tmp1 = new snakeNode(X, Y, parent);
    SnakeBody.push_back(tmp1);
    Map.Grid[X][Y]->Type = 2; //2代表蛇
    snakeNode* tmp2 = new snakeNode(next_X, next_Y, parent);
    SnakeBody.push_back(tmp2);
    Map.Grid[next_X][next_Y]->Type = 2;

    // 蛇头初始化
    if (isBoy) (*SnakeBody.begin())->label->setStyleSheet("background:rgb(240, 120, 120)");
    else  (*SnakeBody.begin())->label->setStyleSheet("background:rgb(120, 240, 120)");
    (*SnakeBody.begin())->label->setGeometry((*SnakeBody.begin())->x * (Parameters.gridLength + Parameters.gridGap), (*SnakeBody.begin())->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
    (*SnakeBody.begin())->label->hide();
    // 蛇尾初始化
    if (isBoy) (*SnakeBody.rbegin())->label->setStyleSheet("background:rgb(240, 120, 120)");
    else  (*SnakeBody.rbegin())->label->setStyleSheet("background:rgb(120, 240, 120)");
    (*SnakeBody.rbegin())->label->setGeometry((*SnakeBody.rbegin())->x * (Parameters.gridLength + Parameters.gridGap), (*SnakeBody.rbegin())->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
    (*SnakeBody.rbegin())->label->hide();

    // 分数初始化
    QString bs = isBoy ? "BoyScore: " : "GirlScore: ";
    bs.append(QString::number(isBoy ? Boy.score = 0 : Girl.score = 0));
    QFont Font("Microsoft YaHei", 16, 75);
    if (isBoy)
    {
        if (Boy.Score) Boy.Score->clear();
        else Boy.Score = new QLabel(parent);
        Boy.Score->setText(bs);
        Boy.Score->setGeometry(750, -50, 400, 400);
        Boy.Score->setStyleSheet("color:#000000;");
        Boy.Score->setFont(Font);
        Boy.Score->show();
        Boy.Score->repaint();
    } else {
        if (Girl.Score) Girl.Score->clear();
        else Girl.Score = new QLabel(parent);
        Girl.Score->setText(bs);
        Girl.Score->setGeometry(750, 50, 400, 400);
        Girl.Score->setStyleSheet("color:#000000;");
        Girl.Score->setFont(Font);
        Girl.Score->hide();
        if (userNumber == 2) {
            Girl.Score->show();
            Girl.Score->repaint();
        }
    };

    // 生命值初始化
    bs = isBoy ? "BoyLife: " : "GirlLife: ";
    bs.append(QString::number(isBoy ? Boy.life = 3 : Girl.life = 3));
    QFont _Font("Microsoft YaHei", 16, 75);
    if (isBoy)
    {
        if (Boy.Life) Boy.Life->clear();
        else Boy.Life = new QLabel(parent);
        Boy.Life->setText(bs);
        Boy.Life->setGeometry(750, 0, 400, 400);
        Boy.Life->setStyleSheet("color:#ff0000;");
        Boy.Life->setFont(Font);
        Boy.Life->show();
        Boy.Life->repaint();
    } else {
        if (Girl.Life) Girl.Score->clear();
        else Girl.Life = new QLabel(parent);
        Girl.Life->setText(bs);
        Girl.Life->setGeometry(750, 100, 400, 400);
        Girl.Life->setStyleSheet("color:#ff0000;");
        Girl.Life->setFont(Font);
        Girl.Life->hide();
        if (userNumber == 2) {
            Girl.Life->show();
            Girl.Life->repaint();
        }
    }
    return;
}
void snake::move(QWidget *parent, int Dir, int isBoy)
{
    int cur_Dir = isBoy ? Boy.Dir : Girl.Dir;

    if (!((Dir % 2) ^ (cur_Dir % 2))) return; // 判断是否同方向按键
    else (isBoy ? Boy.Dir : Girl.Dir) = Dir; // 改变方向

    return;
}
void snake::walk(QWidget *parent, int isBoy)
{
    //计算下一个节点坐标并更新

    int X = isBoy ? (*Boy.SnakeBody.begin())->x : (*Girl.SnakeBody.begin())->x;
    int Y = isBoy ? (*Boy.SnakeBody.begin())->y : (*Girl.SnakeBody.begin())->y;

    int next_X = X, next_Y = Y;
    switch (Dir) {
        case 1: next_Y--; break;
        case 2: next_X++; break;
        case 3: next_Y++; break;
        case 4: next_X--; break;
    }
    //超出边界特判
    if (next_X < 0) next_X += Parameters.X_Border;
    else if (next_X > Parameters.X_Border - 1) next_X -= Parameters.X_Border;
    if (next_Y < 0) next_Y += Parameters.Y_Border;
    else if (next_Y > Parameters.Y_Border - 1) next_Y -= Parameters.Y_Border;

    //尾巴先走再走头
    Map.Grid[(*SnakeBody.rbegin())->x][(*SnakeBody.rbegin())->y]->Type = 1;
    (*SnakeBody.rbegin())->label->hide();
    SnakeBody.pop_back();

    if (Map.Grid[next_X][next_Y]->Type == 2) //撞蛇
        failed(parent, isBoy);
    else if (Map.Grid[next_X][next_Y]->Type <= 0) //撞墙
        failed(parent, isBoy);

    //重新计算下一个节点坐标并更新
    X = isBoy ? (*Boy.SnakeBody.begin())->x : (*Girl.SnakeBody.begin())->x;
    Y = isBoy ? (*Boy.SnakeBody.begin())->y : (*Girl.SnakeBody.begin())->y;
    next_X = X, next_Y = Y;
    switch (Dir) {
        case 1: next_Y--; break;
        case 2: next_X++; break;
        case 3: next_Y++; break;
        case 4: next_X--; break;
    }
    //超出边界特判
    if (next_X < 0) next_X += Parameters.X_Border;
    else if (next_X > Parameters.X_Border - 1) next_X -= Parameters.X_Border;
    if (next_Y < 0) next_Y += Parameters.Y_Border;
    else if (next_Y > Parameters.Y_Border - 1) next_Y -= Parameters.Y_Border;

    //将新头结点放入
    snakeNode *tmp = new snakeNode(next_X, next_Y, parent);
    SnakeBody.push_front(tmp);
    Map.Grid[next_X][next_Y]->Type = 2;
    (*SnakeBody.begin())->label->setGeometry((*SnakeBody.begin())->x * (Parameters.gridLength + Parameters.gridGap), (*SnakeBody.begin())->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);

    //是否吃到果实
    getFruit(parent, isBoy);

    return;
}
void snake::drawSnake(QWidget *parent, bool isBoy)
{
    std::deque<snakeNode*>::iterator iter = SnakeBody.begin();

    //改变蛇头颜色
    if (isBoy) (*iter)->label->setStyleSheet("background:#0066CC");
    else  (*iter)->label->setStyleSheet("background:#FF9900");
    (*iter)->label->show();
    iter++;

    while(iter != SnakeBody.end()){
        if (isBoy) (*iter)->label->setStyleSheet("background:#0099CC");
        else  (*iter)->label->setStyleSheet("background:#FFCC00");
        (*iter)->label->show();
        iter++;
    };

    return;
}
bool snake::getFruit(QWidget *parent, int isBoy)
{
    //得到蛇头坐标
    int head_X = isBoy ? (*Boy.SnakeBody.begin())->x : (*Girl.SnakeBody.begin())->x;
    int head_Y = isBoy ? (*Boy.SnakeBody.begin())->y : (*Girl.SnakeBody.begin())->y;

    std::deque<map::mapNode*>::iterator iter = Map.Food.begin();
    while(iter != Map.Food.end())
    {
        //如果吃到了果实
        if ((*iter)->x == head_X && (*iter)->y == head_Y)
        {
            //更新地图速度
            if (Map.moveSpeed >= 100) Map.moveSpeed -= 5;
            //获取蛇尾
            int X = isBoy ? (*Boy.SnakeBody.rbegin())->x : (*Girl.SnakeBody.rbegin())->x;
            int Y = isBoy ? (*Boy.SnakeBody.rbegin())->y : (*Girl.SnakeBody.rbegin())->y;

            //加长蛇尾
            int next_X = X, next_Y = Y;
            switch (Dir) {
            case 1: next_Y++; break;
            case 2: next_X--; break;
            case 3: next_Y--; break;
            case 4: next_X++; break;
            }
            //超出边界特判
            if (next_X < 0) next_X += Parameters.X_Border;
            else if (next_X > Parameters.X_Border - 1) next_X -= Parameters.X_Border;
            if (next_Y < 0) next_Y += Parameters.Y_Border;
            else if (next_Y > Parameters.Y_Border - 1) next_Y -= Parameters.Y_Border;

            snakeNode* tmp = new snakeNode(next_X, next_Y, parent);
            SnakeBody.push_back(tmp);
            Map.Grid[next_X][next_Y]->Type = 2;

            //删除果实
            (*iter)->label->hide();
            (*iter)->label->clear();
            int Type = (*iter)->Type;
            Map.Food.erase(iter);

            //根据果实类型进行相应操作
            if (Type == 3)
            {
                score++; //普通加分果实
                updateScore(parent, isBoy);
            } else if (Type == 4) { //加生命值果实
                life++;
                updateScore(parent, isBoy);
            } else if (Type == 5) {
                //地图速度更新，随机增加或减少地图速度
                srand(time(NULL));
                if ((rand() & 1) == 1)
                    if (Map.moveSpeed > 100) Map.moveSpeed -= 150;
                    else Map.moveSpeed += 50;
                else {
                    if (Map.moveSpeed < 800) Map.moveSpeed += 100;
                    else Map.moveSpeed -= 50;
                }
            } else if (Type == 6) {
                score += 5;
                if (!isAI) Map.drawMap(parent, true, true); //更新墙体
                updateScore(parent, isBoy);
            }

            //生成相应新食物
            Map.createFruit(parent, Type, 1);

            return true;
        }
        iter++;
    }
    return false;
}
bool snake::failed(QWidget *parent, int isBoy)
{
    if (isBoy) {
        Boy.life--;
        Boy.updateScore(parent, isBoy);

        //死亡恢复时调转方向
        reverse(Boy.SnakeBody.begin(), Boy.SnakeBody.end());
        int head_X = (*Boy.SnakeBody.begin())->x, head_Y = (*Boy.SnakeBody.begin())->y;
        if (!Boy.AIcrash(parent, head_X, head_Y - 1)) Boy.Dir = 1;
        else if (!Boy.AIcrash(parent, head_X + 1, head_Y)) Boy.Dir = 2;
        else if (!Boy.AIcrash(parent, head_X, head_Y + 1)) Boy.Dir = 3;
        else if (!Boy.AIcrash(parent, head_X - 1, head_Y)) Boy.Dir = 4;
        else Boy.Dir = 1 + (Boy.Dir + 1) % 4;
    } else {
        Girl.life--;
        Girl.updateScore(parent, isBoy);

        reverse(Girl.SnakeBody.begin(), Girl.SnakeBody.end());
        int head_X = (*Girl.SnakeBody.begin())->x, head_Y = (*Girl.SnakeBody.begin())->y;
        if (!Girl.AIcrash(parent, head_X, head_Y - 1)) Girl.Dir = 1;
        else if (!Girl.AIcrash(parent, head_X + 1, head_Y)) Girl.Dir = 2;
        else if (!Girl.AIcrash(parent, head_X, head_Y + 1)) Girl.Dir = 3;
        else if (!Girl.AIcrash(parent, head_X - 1, head_Y)) Girl.Dir = 4;
        else Girl.Dir = 1 + (Girl.Dir + 1) % 4;
    }


    if (Boy.life == 0 || Girl.life == 0)    //如果有哪一方生命值为零，则游戏结束
    {
        if (isBoy){
            if (userNumber == 1)
                Map.Final->setText("You Fail!");
            else
                Map.Final->setText("Girl Win!");
        } else {
            if (userNumber == 1)
                Map.Final->setText("You Fail!");
            else
                Map.Final->setText("Boy Win!");
        };
        //设置开关
        Button.start->setEnabled(true);
        Button.save->setEnabled(false);
        Button.read->setEnabled(false);
        Button.pause->setEnabled(false);

        Map.timer.stop();  //计时器停止
        for (int i = 0; i <= 10; i++ ) Map.Final->raise(); //置于顶层
        Map.Final->show();

        return true;
    }
    return false;
};

void snake::copy(QWidget *parent, snake &res_Snake, bool isBoy)
{
    // 清空上一次记录
    if (!res_Snake.SnakeBody.empty()) res_Snake.SnakeBody.clear();
    // 放入本次记录的蛇身
    std::deque<snake::snakeNode*>::iterator iter = this->SnakeBody.begin();
    while(iter != this->SnakeBody.end()){
        snake::snakeNode* M = new snakeNode(*iter, parent);
        res_Snake.SnakeBody.push_back(M);
        Map.Grid[M->x][M->y]->Type = 2;
        //蛇身设置
        if (isBoy) (*iter)->label->setStyleSheet("background:#0099CC");
        else  (*iter)->label->setStyleSheet("background:#FFCC00");

        iter++;
    };

    // 蛇头设置
    if (isBoy) (*res_Snake.SnakeBody.begin())->label->setStyleSheet("background:#0066CC");
    else  (*res_Snake.SnakeBody.begin())->label->setStyleSheet("background:#FF9900");

    // 方向设置
    res_Snake.Dir = this->Dir;
    // 分数设置
    if (res_Snake.Score) {
        res_Snake.Score->hide();
        res_Snake.Score->clear();
    } else res_Snake.Score = new QLabel(parent);

    res_Snake.score = score;
    QString bs = isBoy ? "BoyScore: " : "GirlScore: ";
    bs.append(QString::number(res_Snake.score));
    QFont Font("Microsoft YaHei", 16, 75);
    res_Snake.Score->setText(bs);
    isBoy ? Score->setGeometry(750, -50, 400, 400) : Score->setGeometry(750, 50, 400, 400);
    res_Snake.Score->setFont(Font);
    res_Snake.Score->hide();

    //生命值设置
    if (res_Snake.Life) {
        res_Snake.Life->hide();
        res_Snake.Life->clear();
    } else res_Snake.Life = new QLabel(parent);

    res_Snake.life = life;
    bs = isBoy ? "BoyLife: " : "GirlLife: ";
    bs.append(QString::number(res_Snake.life));
    QFont _Font("Microsoft YaHei", 16, 75);
    res_Snake.Life->setText(bs);
    isBoy ? Life->setGeometry(750, 0, 400, 400) : Life->setGeometry(750, 100, 400, 400);
    res_Snake.Life->setFont(Font);
    res_Snake.Life->hide();

    return;
}

void snake::updateScore(QWidget* parent, bool isBoy) {
    QString s = isBoy ? "BoyScore: " : "GirlScore: ";
    s.append(QString::number(score));
    Score->hide();
    Score->clear();
    Score->setText(s);
    Score->show();

    QString bs = isBoy ? "BoyLife: " : "GirlLife: ";
    bs.append(QString::number(isBoy ? Boy.life : Girl.life));
    Life->hide();
    Life->clear();
    Life->setText(bs);
    Life->show();
}
void snake::clearSnake(QWidget *parent)
{
    if (SnakeBody.empty()) return;

    //将原蛇身全部去掉
    std::deque<snakeNode*>::iterator iter = SnakeBody.begin();
    while (iter != SnakeBody.end()) {
        (*iter)->label->hide();
        Map.Grid[(*iter)->x][(*iter)->y]->Type = 1;
        iter++;
    }
    //Map.Grid[(*SnakeBody.begin())->x][(*SnakeBody.begin())->y]->Type = 1;
    //Map.Grid[(*SnakeBody.rbegin())->x][(*SnakeBody.rbegin())->y]->Type = 1;

    Map.drawMap(parent);
}
void snake::saveSnake(QWidget* parent, QTextStream& In)
{
    std::deque<snakeNode*>::iterator iter = SnakeBody.begin();

    In << SnakeBody.size() << Qt::endl; //读档时使用
    while(iter != SnakeBody.end()){
        In << (*iter)->x << Qt::endl;
        In << (*iter)->y << Qt::endl;
        iter++;
    }
    return;
}
void snake::regainSnake(QWidget* parent, QTextStream& In)
{
    if (!SnakeBody.empty()) SnakeBody.clear();

    int cnt = In.readLine().toInt();    //先获取原蛇长度，方便后续读取

    while(cnt--){
        int x = In.readLine().toInt(), y = In.readLine().toInt();
        snakeNode* tmp = new snakeNode(x, y, parent);
        SnakeBody.push_back(tmp);
        Map.Grid[x][y]->Type = 2;
    }
    return;
}

bool snake::getAIroad(QWidget *parent)
{
    Map.moveSpeed = 20;     //AI蛇速度

    //获取蛇头坐标
    int head_X = (*SnakeBody.begin())->x, head_Y = (*SnakeBody.begin())->y;

    //随机化
    srand(time(NULL));

    std::deque<map::mapNode*>::iterator iter = Map.Food.begin();
    int res_X = (*Map.Food.begin())->x, res_Y = (*Map.Food.begin())->y, res_dis = 0x7fffffff;

    if ((AIdown == 0 && AIright == 0))  //如果吃到了食物则计算下个食物的坐标
    {
        while(iter != Map.Food.end())
        {
            if (((*iter)->x - head_X) * ((*iter)->x - head_X) +
                    ((*iter)->y - head_Y) * ((*iter)->y - head_Y) < res_dis) {
                res_dis = ((*iter)->x - head_X) * ((*iter)->x - head_X) +
                        ((*iter)->y - head_Y) * ((*iter)->y - head_Y);
                res_X = (*iter)->x;
                res_Y = (*iter)->y;
            }

            iter++;
        }
    };

    AIright = res_X - head_X;
    AIdown = res_Y - head_Y;

    //先判断能否走，如果能一定走，走了则跳出
    //然后判断其他方向能否走，如果能则一定走，走了则跳出
    //否则不改变方向，让蛇损失一条命
    if (AIright > 0 && !AIcrash(parent, head_X + 1, head_Y)) {
        Dir = 2; //向右走
        AIright--;
    } else if (AIright < 0 && !AIcrash(parent, head_X - 1, head_Y)) {
        Dir = 4; //向左走
        AIright++;
    } else if (AIdown > 0 && !AIcrash(parent, head_X, head_Y + 1)) {
        Dir = 3; //向下走
        AIdown--;
    } else if (AIdown < 0 && !AIcrash(parent, head_X, head_Y - 1)) {
        Dir = 1; //向上走
        AIdown++;
    }
       else if (!AIcrash(parent, head_X + 1, head_Y)) {
        Dir = 2; //向右走
        AIright--;
     } else if (!AIcrash(parent, head_X - 1, head_Y)) {
        Dir = 4; //向左走
        AIright++;
     } else if (!AIcrash(parent, head_X, head_Y + 1)) {
         Dir = 3; //向下走
         AIdown--;
     } else if (!AIcrash(parent, head_X, head_Y - 1)) {
         Dir = 1; //向上走
         AIdown++;
     } else {
        return false;
     }

    return true;
}
bool snake::AIcrash(QWidget* parent, int x, int y)
{
    /*if (x < 0 || x > Parameters.X_Border - 1
        || y < 0 || y > Parameters.Y_Border - 1) return true;*/

    //穿出边界特判
    if (x < 0) x += Parameters.X_Border;
    else if (x > Parameters.X_Border - 1) x -= Parameters.X_Border;
    if (y < 0) y += Parameters.Y_Border;
    else if (y > Parameters.Y_Border - 1) y -= Parameters.Y_Border;

    //判断是否撞蛇
    std::deque<snake::snakeNode*>::iterator iter = Boy.SnakeBody.begin();
    while(iter != Boy.SnakeBody.end())
    {
        if ((*iter)->x == x && (*iter)->y == y) return true;
        iter++;
    };
    iter = Girl.SnakeBody.begin();
    while(iter != Girl.SnakeBody.end())
    {
        if ((*iter)->x == x && (*iter)->y == y) return true;
        iter++;
    }
    //判断是否撞墙
    if (Map.Grid[x][y]->Type <= 0) return true;

    return false;
}
