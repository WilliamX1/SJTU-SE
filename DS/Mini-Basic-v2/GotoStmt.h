#ifndef GOTOSTMT_H
#define GOTOSTMT_H

#include "ConstantExp.h"
#include "Expression.h"
#include "Statement.h"

class GotoStmt: public Statement {
private:
    Expression* exp;
    QString _contents;
public:
    GotoStmt(QString stmt);
    /*override*/
    void printToScreen(QTextBrowser* ui_textbrowser);
    /*override*/
    void getSyntaxTree(); //获得语句与语法树
    /*override*/
    bool checkUserInputValid(QString& errorName); //检查用户输入是否合法
    /*override*/
    /*标准化字符串*/
    void normalLize();
};


#endif // GOTOSTMT_H
