#ifndef ENDSTMT_H
#define ENDSTMT_H

#include "Statement.h"

class EndStmt: public Statement {
private:
    QString _contents;
public:
    EndStmt(QString stmt);
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

#endif // ENDSTMT_H
