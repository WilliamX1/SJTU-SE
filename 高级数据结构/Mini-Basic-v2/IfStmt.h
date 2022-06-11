#ifndef IFSTMT_H
#define IFSTMT_H

#include "CompoundExp.h"
#include "ConstantExp.h"
#include "Expression.h"
#include "IdentifierExp.h"
#include "Statement.h"

class IfStmt: public Statement {
private:
    Expression* exp1;
    Expression* exp2;
    Expression* n;
    QString _contents;
public:
    IfStmt(QString stmt);
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

#endif // IFSTMT_H
