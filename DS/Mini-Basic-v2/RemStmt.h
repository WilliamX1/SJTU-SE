#ifndef REMSTMT_H
#define REMSTMT_H

#include "Statement.h"

class RemStmt: public Statement {
private:
    QString _contents;
public:
    RemStmt(QString stmt);
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

#endif // REMSTMT_H
