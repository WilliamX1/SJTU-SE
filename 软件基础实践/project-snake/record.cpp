#include "record.h"

record Record[3];
extern map Map;
extern snake Girl, Boy;
extern int userNumber;

void record::copySave(QWidget *parent)
{
    flag = true;    //代表赋予本结构体存档，读档时需判此flag

    //创建新的Map和Snake来存档
    copyMap = new map;
    copyBoy = new snake;
    if (userNumber == 2) copyGirl = new snake;

    //将现在值赋给存档
    Map.copy(parent, *copyMap);
    Boy.copy(parent, *copyBoy, true);
    if (userNumber == 2) Girl.copy(parent, *copyGirl, false);
    return;
}
void record::readSave(QWidget *parent, int idx)
{
    //将存档值赋回现在
    copyMap->copy(parent, *&Map);

    copyBoy->copy(parent, Boy, true);
    Boy.Score->show();
    Boy.Life->show();

    if (userNumber == 2) {
        copyGirl->copy(parent, Girl, false);
        Girl.Score->show();
        Girl.Life->show();
    }
    Map.drawMap(parent);
    return;
}
