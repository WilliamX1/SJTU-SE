#ifndef INPUTSTMT_H
#define INPUTSTMT_H

#include "Expression.h"
#include "IdentifierExp.h"
#include "Statement.h"

class InputStmt: public Statement {
private:
    Expression* exp;
    QString _contents;
public:
    InputStmt(QString stmt);
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

#endif // INPUTSTMT_H
