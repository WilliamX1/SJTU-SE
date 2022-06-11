#ifndef STATEMENT_H
#define STATEMENT_H

#include <QChar>
#include <QString>
#include <QTextBrowser>
#include "Expression.h"

enum cmdCases {
    REM, LET, PRINT, PRINTF, INPUT, INPUTS, GOTO, IF, END, Error
};

class Statement {
protected:
    int _lineNumber;   //行号
    QString _cmdStr; //命令类型
public:
    QString stmt;    //语句
    cmdCases _cmd; //命令类型
    std::vector<Expression*> exps; //Printf专用
    Statement(QString str): stmt(str) {
        /*转换行号*/
        _lineNumber = stmt.section(' ', 0, 0).toInt();
        /*转换命令*/
        _cmdStr = str.section(' ', 1, 1);
        if (_cmdStr == "REM")
            _cmd = REM;
        else if (_cmdStr == "LET")
            _cmd = LET;
        else if (_cmdStr == "PRINT")
            _cmd = PRINT;
        else if (_cmdStr == "PRINTF")
            _cmd = PRINTF;
        else if (_cmdStr == "INPUT")
            _cmd = INPUT;
        else if (_cmdStr == "INPUTS")
            _cmd = INPUTS;
        else if (_cmdStr == "GOTO")
            _cmd = GOTO;
        else if (_cmdStr == "IF")
            _cmd = IF;
        else if (_cmdStr == "END")
            _cmd = END;
        else {
            _cmd = Error;
        }
    };
    virtual void getSyntaxTree() = 0; //获得语句与语法树
    virtual void printToScreen(QTextBrowser* ui_textbrowser) = 0;
    virtual bool checkUserInputValid(QString& errorName) = 0;
    QString getTheFirstElem(){
        return stmt.section(' ', 2, -1).simplified();
    };
    /*标准化字符串*/
    virtual void normalLize() = 0;
};
#endif // STATEMENT_H
