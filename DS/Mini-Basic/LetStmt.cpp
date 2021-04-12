#include "LetStmt.h"

LetStmt::LetStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
}
/*override*/
void LetStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void LetStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool LetStmt::checkUserInputValid(QString& errorName) {
    int equalIdx = _contents.indexOf('=');
    if (equalIdx == -1) {
        errorName = "Error: 'LET' should have a '=' identifier!";
        return false;
    } else {
        /*左边标识符检查*/
        /*注意去除头尾空格*/
        /*判断第一个参数是否存在*/
        /*第一个参数必须是Identifier*/
        if (_contents.section('=', 0, 0).simplified() == "") {
            errorName = "Error: Missing Parameter!";
            return false;
        }
        expLeft = new IdentifierExp(_contents.section('=', 0, 0).simplified());
        if (!expLeft->checkUserInputValid(errorName)) {
            delete expLeft;
            return false;
        } else {};
        /*右边标识符检查*/
        /*判断第二个参数是否存在和类型*/
        if (_contents.section('=', 1, -1).trimmed() == "") {
            errorName = "Error: Missing Parameter!";
            return false;
        }
        switch (Expression::whichExpMightBe(_contents.section('=', 1, -1))) {
        case Compound: expRight = new CompoundExp(_contents.section('=', 1, -1));break;
        case Constant: expRight = new ConstantExp(_contents.section('=', 1, -1)); break;
        case Identifier: expRight = new IdentifierExp(_contents.section('=', 1, -1)); break;
        }
        if (!expRight->checkUserInputValid(errorName)) {
            delete expLeft;
            delete expRight;
            return false;
        } else {};
    }
    return true;
}; //检查用户输入是否合法
/*返回等式左变量*/
/*override*/
/*override*/
/*标准化字符串*/
void LetStmt::normalLize(){
    /*防止首字母就是运算符导致数组越界*/
    _contents = " " + _contents.simplified() + " ";
    for (int i = 0; _contents[i] != '\0'; i++){
        /*特殊考虑乘方*/
        if (_contents[i] == '*' && (i + 1) <= _contents.length() && _contents[i + 1] == '*') {
            _contents = _contents.mid(0, i) + " " + _contents[i] + _contents[i + 1] + " " + _contents.mid(i + 2, -1);
            i += 3;
        }
        /*凡是运算符、括号前后均加空格*/
        else if (_contents[i] == '+' || _contents[i] == '-' || _contents[i] == '*' || _contents[i] == '/' || _contents[i] == '='
                 || _contents[i] == '>' || _contents[i] == '<' || _contents[i] == '(' || _contents[i] == ')') {
            _contents = _contents.mid(0, i) + " " + _contents[i] + " " + _contents.mid(i + 1, -1);
            i += 2;
        } else continue;
    }
    _contents = _contents.simplified();
    /*规定负数必须用括号括起来*/
    for (int i = 0; _contents[i] != '\0'; i++) {
        if (_contents[i] == '-') {
            if (i - 2 < 0 || _contents[i - 2] == '(') {
                _contents[i] = ' ';
                _contents[i + 1] = '-';
                i++;
            }
        }
    }
    _contents = _contents.simplified();
    return;
}
