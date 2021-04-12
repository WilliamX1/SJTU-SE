#include "Expression.h"

ExpCases Expression::whichExpMightBe(QString str){
    /*如果中间存在空格*/
    str = str.simplified();
    if (str.section(' ', 0, 0) != str.section(' ', -1, -1)) return Compound;
    else if (str.toInt() ||
             (str.length() == 1 && str[0] == '0')) return Constant;
    else return Identifier;
};
