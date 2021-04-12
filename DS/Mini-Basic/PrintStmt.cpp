#include "PrintStmt.h"

PrintStmt::PrintStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
};
/*override*/
void PrintStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void PrintStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool PrintStmt::checkUserInputValid(QString& errorName) {
    /*缺少参数*/
    if (_contents == "") {
        errorName = "Error: Missing Parameter!";
        return false;
    }
    /*根据不同的exp创建相应*/
    switch (Expression::whichExpMightBe(_contents)) {
    case Compound: exp = new CompoundExp(_contents); break;
    case Constant: exp = new ConstantExp(_contents); break;
    case Identifier: exp = new IdentifierExp(_contents); break;
    }

    if (!exp->checkUserInputValid(errorName)) {
        delete exp;
        return false;
    } else return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void PrintStmt::normalLize(){
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
