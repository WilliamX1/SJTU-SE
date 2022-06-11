#ifndef LETSTMT_H
#define LETSTMT_H

#include "CompoundExp.h"
#include "ConstantExp.h"
#include "Expression.h"
#include "IdentifierExp.h"
#include "Statement.h"
#include "StringExp.h"
#include <iostream>

class LetStmt: public Statement{
private:
    /*左右表达式*/
    Expression* expLeft;
    Expression* expRight;
    QString _contents;
public:
    LetStmt(QString stmt);
    /*override*/
    void printToScreen(QTextBrowser* ui_textbrowser);
    /*override*/
    void getSyntaxTree(); //获得语句与语法树
    /*override*/
    bool checkUserInputValid(QString& errorName);
    /*override*/
    /*标准化字符串*/
    void normalLize();
};

#endif // LETSTMT_H
