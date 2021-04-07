#ifndef PARAMETERS_H
#define PARAMETERS_H

class
{
public:
    const int X_Border = 16;
    const int Y_Border = 16;

    const int Boy_Start_X = 4;
    const int Boy_Start_Y = 3;
    const int Girl_Start_X = 10;
    const int Girl_Start_Y = 10;

    const int gridGap = 3;
    const int gridLength = 42;
    const int widgetWidth = 980 + 150;
    const int widgetHeight = X_Border * (gridGap + gridLength);

    bool isEditUserNumber = true;  //是否进入编辑人数状态
    bool isEditWall = true;  //是否进入编辑墙状态
    bool isEditBean = true;  //是否进入编辑豆子状态
}Parameters;
#endif // PARAMETERS_H
