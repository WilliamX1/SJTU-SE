#include "StringExp.h"

StringExp::StringExp(QString str): Expression(str)
{
    _var = str.simplified();
};
/*override*/
bool StringExp:: checkUserInputValid(QString &errorName)
{
    int len = _var.length();
    if (len < 2) return false; //至少要有两头的引号
    if (_var.count('\"') != 2 && _var.count('\'') != 2) return false;

    if (_var.indexOf('\"') == 0 && _var.lastIndexOf('\"') == len - 1) return !_var.contains('\'');
    if (_var.indexOf('\'') == 0 && _var.lastIndexOf('\'') == len - 1) return !_var.contains('\"');
    return false;
};
/*override*/
StringExp::~StringExp()
{

}
