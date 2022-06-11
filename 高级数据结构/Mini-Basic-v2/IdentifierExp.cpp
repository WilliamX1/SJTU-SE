#include "IdentifierExp.h"

IdentifierExp::IdentifierExp(QString str): Expression(str){
    _var = str.simplified();
};
/*override*/
bool IdentifierExp::checkUserInputValid(QString& errorName){
    /*判断首字母是否是小写英文字母或大写英文字母或下划线*/
    if (('a' <= _var[0] && _var[0] <= 'z')
            || ('A' <= _var[0] && _var[0] <= 'Z')
                || (_var[0] == '_')) {}
    else {
        errorName = "Error: The first letter of identifier is invalid!";
        return false;
    }
    /*判断长度*/
    if (_var.length() < 80) {}
    else {
        errorName = "Error: The length of identifier exceeds the limit!";
        return false;
    };
    /*判断其他字符是否合法*/
    int idx = _var.length();
    for (int i = 0; i < idx; i++)
        if (('a' <= _var[i] && _var[i] <= 'z')
                || ('A' <= _var[i] && _var[i] <= 'Z')
                    || ('0' <= _var[i] && _var[i] <= '9')
                        ||(_var[i] == '_')) {}
        else {
            errorName = "Error: The letter of identifier is invalid!";
            return false;
        };

    return true;
};
/*override*/
IdentifierExp::~IdentifierExp(){};
