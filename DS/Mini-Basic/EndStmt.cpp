#include "EndStmt.h"

EndStmt::EndStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
}
/*override*/
void EndStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void EndStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool EndStmt::checkUserInputValid(QString& errorName) {
    /*无参数*/
    if (_contents != "") {
        errorName = "Error: Too many parameters for command 'END'!\t";
        return false;
    } else return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void EndStmt::normalLize(){
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
