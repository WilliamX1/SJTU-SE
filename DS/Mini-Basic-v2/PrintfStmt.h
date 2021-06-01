#ifndef PRINTFSTMT_H
#define PRINTFSTMT_H

#include <vector>
#include "CompoundExp.h"
#include "ConstantExp.h"
#include "Expression.h"
#include "IdentifierExp.h"
#include "Statement.h"
#include "StringExp.h"

class PrintfStmt: public Statement{
private:
    QString _contents;
public:
    PrintfStmt(QString stmt);
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

#endif // PRINTFSTMT_H
