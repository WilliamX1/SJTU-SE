#include "map.h"
map Map;
extern snake Boy, Girl;
extern bool isAI;

map::map(){
    //将所有节点设置为空节点，防止复制出错
    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
            Grid[i][j] = nullptr;
    return;
};
void map::init(QWidget *parent)
{
    /*初始化游戏*/
    if (isAI) up_down_Wall = left_right_Wall = false;   //AI模式不需要墙体

    moveSpeed = 400;  //地图更新时间，每400msec更新一次

    //更新地图所有结点
    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
        {
            if (Grid[i][j]) delete Grid[i][j];  //若有读档行为，则删除原来结点
            Grid[i][j] = new mapNode(i, j, Parameters.gridLength, parent);
            Grid[i][j]->Type = 1; // 1代表地
            Grid[i][j]->label->setGeometry(Grid[i][j]->x * (Parameters.gridLength + Parameters.gridGap), Grid[i][j]->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
            Grid[i][j]->label->hide();
        }

    //更新右侧“贪吃蛇”标题
    if (Title) {
        Title->hide();
        Title->clear();
    }
    else Title = new QLabel(parent);
    Title->setText("贪 吃 蛇\n大 作 战");
    Title->setGeometry(770, -140, 400, 400);
    Title->setStyleSheet("color:#00ccff;");
    QFont Font("Trebuchet MS", 20, 115);
    Title->setFont(Font);
    Title->show();

    //更新结束语
    if (Final) {
        Final->hide();
        Final->clear();
    }
    else Final = new QLabel(parent);
    Final->setText("Boy Win!");
    Final->setGeometry(300, 120, 400, 400);
    Final->setStyleSheet("color:#ff6600;");
    QFont Font1("Monotype Corsiva", 60, 75);
    Final->setFont(Font1);
    Final->hide();

    //清空食物链表并生成各类食物
    if (!Food.empty()) Food.clear();
    if (!isAI) {
        createFruit(parent, 3, normalBean); //生成6个普通加分食物
        createFruit(parent, 4, lifeBean); //生成1个增加生命值食物
        createFruit(parent, 5, speedBean); //生成2个增加/减少速度食物
        createFruit(parent, 6, wallBean); //生成3个改变墙体状态食物
    } else createFruit(parent, 4, 3);

    return;
}
void map::drawMap(QWidget *parent, bool LR, bool UD) //是否改变墙状态
{
    //根据参数改变墙状态
    if (LR) {
        left_right_Wall = !left_right_Wall; //LR为真代表将左右墙体状态置反
    }
    if (UD) up_down_Wall = !up_down_Wall;   //UD为真代表将上下墙体状态置反

    //修改对应节点状态
    for (int j = 1; j < Parameters.Y_Border - 1; j++)
        Grid[0][j]->Type = Grid[Parameters.X_Border - 1][j]->Type = !up_down_Wall;
    for (int i = 1; i < Parameters.X_Border - 1; i++)
        Grid[i][0]->Type = Grid[i][Parameters.Y_Border - 1]->Type = !left_right_Wall;

    //四个角节点永远不改变
    Grid[0][0]->Type = Grid[0][Parameters.Y_Border - 1]->Type = -1;
    Grid[Parameters.X_Border - 1][0]->Type = Grid[Parameters.X_Border - 1][Parameters.Y_Border - 1]->Type = -1;

    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
        {
            Grid[i][j]->label->hide();
            Grid[i][j]->label->clear();
            switch (Grid[i][j]->Type){
            //-1是角，0是墙，1是地，2是蛇，3是果实，4是生命值，5是加速/减速，6是改变墙体
            case -1: Grid[i][j]->label->setStyleSheet("background:rgb(60,60,60)"); break;
            case 0: Grid[i][j]->label->setStyleSheet("background:rgb(120,120,120)"); break;
            case 1: case 2: Grid[i][j]->label->setStyleSheet("background:#99CC99"); break;
            case 3: Grid[i][j]->label->setStyleSheet("background:rgb(0,0,0)"); break;
            case 4: Grid[i][j]->label->setStyleSheet("background:rgb(250,0,0)"); break;
            case 5: Grid[i][j]->label->setStyleSheet("background:#9933FF"); break;
            case 6: Grid[i][j]->label->setStyleSheet("background:rgb(150,150,150)"); break;
            default: break;
            }
            Grid[i][j]->label->show();
        }
    return;
};
void map::createFruit(QWidget *parent, int Type, int num)
{
    if (num < 0) {  //删减食物
        std::deque<map::mapNode*>::iterator iter = Food.begin();
        while (num < 0 && iter != Food.end()) {
            if ((*iter)->Type == Type) {
                (*iter)->label->hide(); //先将原食物的图像隐藏
                Grid[(*iter)->x][(*iter)->y]->Type = 1;
                Food.erase(iter);
                num++;
            }
            iter++;
        }
        return;
    }

    while(num--)    //增加食物
    {
        //生成随机数
        srand(time(NULL));
        int x, y;
        do{
            x = 1 + rand() % (Parameters.X_Border - 2);
            y = 1 + rand() % (Parameters.Y_Border - 2);
        } while(Grid[x][y]->Type != 1); //保证随生成位置对应节点是“地”状态，否则重新生成

        //创建新的食物节点
        mapNode* M = new mapNode(x, y, Parameters.gridLength, parent);
        Food.push_back(M);
        M->Type = Grid[x][y]->Type = Type;
        M->label->setGeometry(M->x * (Parameters.gridLength + Parameters.gridGap), M->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
        M->label->hide();
    }
    return;
}
void map::copy(QWidget *parent, map &res_Map)   //已经废弃不用
{
    //map结点copy
    res_Map.moveSpeed = 500;
    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
        {
            if (res_Map.Grid[i][j]) delete res_Map.Grid[i][j];
            res_Map.Grid[i][j] = new mapNode(this->Grid[i][j], parent);
        }

    //标题copy
    if (res_Map.Title) {
        res_Map.Title->hide();
        res_Map.Title->clear();
    }
    else res_Map.Title = new QLabel(parent);
    res_Map.Title = new QLabel(parent);
    res_Map.Title->setText("贪 吃 蛇\n大 作 战");
    res_Map.Title->setGeometry(770, -140, 400, 400);
    res_Map.Title->setStyleSheet("color:#00ccff;");
    QFont Font("Trebuchet MS", 20, 75);
    res_Map.Title->setFont(Font);
    res_Map.Title->show();

    //map结束语copy
    if (res_Map.Final) {
        res_Map.Final->hide();
        res_Map.Final->clear();
    }
    else res_Map.Final = new QLabel(parent);
    res_Map.Final->setText("Boy Win!");
    res_Map.Final->setGeometry(300, 120, 400, 400);
    res_Map.Final->setStyleSheet("color:#ff6600;");
    QFont Font1("Monotype Corsiva", 40, 75);
    res_Map.Final->setFont(Font1);
    res_Map.Final->hide();

    //map食物copy
    std::deque<map::mapNode*>::iterator iter = Food.begin();
    while(iter != Food.end()){
        mapNode* M = new mapNode(*iter, parent);
        res_Map.Food.push_back(M);
        iter++;
    }

    return;
}

void map::saveMap(QWidget* parent, QTextStream& In)
{
    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
            In << Map.Grid[i][j]->Type << Qt::endl;
    return;
}
void map::regainMap(QWidget* parent, QTextStream& In)
{
    Map.Food.clear();   //将原食物全部清除
    for (int i = 0; i < Parameters.X_Border; i++)
        for (int j = 0; j < Parameters.Y_Border; j++)
        {
            Map.Grid[i][j]->Type = In.readLine().toInt();   //读取类型
            if (3 <= Map.Grid[i][j]->Type && Map.Grid[i][j]->Type <= 6) {
                map::mapNode* M = new map::mapNode(i, j, Parameters.gridLength, parent);
                Map.Food.push_back(M);
                M->Type = Map.Grid[i][j]->Type;
                M->label->setGeometry(M->x * (Parameters.gridLength + Parameters.gridGap), M->y * (Parameters.gridLength + Parameters.gridGap), Parameters.gridLength, Parameters.gridLength);
                M->label->hide();
            }
        }
    return;
}
