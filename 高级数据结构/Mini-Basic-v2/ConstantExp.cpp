#include "ConstantExp.h"

ConstantExp::ConstantExp(QString str): Expression(str) {
    _var = str.simplified();
};
/*override*/
bool ConstantExp::checkUserInputValid(QString& errorName){
    bool flag = false;
    _var.toInt(&flag);
    if (flag) return true;
    else {
        errorName = "Error: Invalid ConstantExp!";
        return false;
    }
};
ConstantExp::~ConstantExp(){

};
