#include "Expression.h"

ExpCases Expression::whichExpMightBe(QString str){
    /*如果中间存在空格*/
    bool flag = false;
    str = str.simplified();
    str.toInt(&flag);
    if (str[0] == '\"' || str[0] == '\'') return String;
    else if (str.count(' ') != 0) return Compound;
    else if (flag) return Constant;
    else return Identifier;
};
