#ifndef PRINTSTMT_H
#define PRINTSTMT_H

#include "CompoundExp.h"
#include "ConstantExp.h"
#include "Expression.h"
#include "IdentifierExp.h"
#include "Statement.h"

class PrintStmt: public Statement{
private:
    Expression* exp;
    QString _contents;
public:
    PrintStmt(QString stmt);
    /*override*/
    void printToScreen(QTextBrowser* ui_textbrowser);
    /*override*/
    void getSyntaxTree(); //获得语句与语法树
    /*override*/
    bool checkUserInputValid(QString& errorName);//检查用户输入是否合法
    /*override*/
    /*标准化字符串*/
    void normalLize();
};

#endif // PRINTSTMT_H
