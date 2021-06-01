#include "CompoundExp.h"

CompoundExp::CompoundExp(QString str): Expression(str) {

};
bool CompoundExp::checkUserInputValid(QString& errorName){
    return true;
};
CompoundExp::~CompoundExp(){

};
