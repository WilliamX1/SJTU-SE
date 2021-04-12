#include "IfStmt.h"

IfStmt::IfStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1);
}
/*override*/
void IfStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void IfStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool IfStmt::checkUserInputValid(QString& errorName) {
    int opIdx = _contents.indexOf(QRegExp("[>=<]"));
    int thenIdx = _contents.indexOf("THEN");
    /*试图匹配小写'THEN'*/
    if (thenIdx == -1) thenIdx = _contents.indexOf("then");

    if (thenIdx == -1) {
        errorName = "Error: 'IF' should have a 'then'!";
        return false;
    } else {
        /*第一个参数检查*/
        /*判断第一个参数是否存在*/
        if (_contents.section(_contents[opIdx], 0, 0).simplified() == "") {
            errorName = "Error: Missing Parameter!";
            return false;
        } else {};
        /*注意去除头尾空格*/
        switch (Expression::whichExpMightBe(_contents.section(_contents[opIdx], 0, 0).simplified())) {
        case Compound: exp1 = new CompoundExp(_contents.section(_contents[opIdx], 0, 0));break;
        case Constant: exp1 = new ConstantExp(_contents.section(_contents[opIdx], 0, 0)); break;
        case Identifier: exp1 = new IdentifierExp(_contents.section(_contents[opIdx], 0, 0)); break;
        }
        if (!exp1->checkUserInputValid(errorName)) {
            delete exp1;
            return false;
        } else {};
        /*第二个参数检查*/
        /*判断第二个参数是否存在和类型*/
        if (_contents.mid(opIdx + 1, thenIdx - opIdx - 1).simplified() == "") {
            errorName = "Error: Missing Parameter!";
            return false;
        } else {};
        /*注意去除头尾空格*/
        switch (Expression::whichExpMightBe(_contents.mid(opIdx + 1, thenIdx - opIdx - 1).simplified())) {
        case Compound: exp2 = new CompoundExp(_contents.mid(opIdx + 1, thenIdx - opIdx - 1).simplified());break;
        case Constant: exp2 = new ConstantExp(_contents.mid(opIdx + 1, thenIdx - opIdx - 1).simplified()); break;
        case Identifier: exp2 = new IdentifierExp(_contents.mid(opIdx + 1, thenIdx - opIdx - 1).simplified()); break;
        }
        if (!exp2->checkUserInputValid(errorName)) {
            delete exp1;
            delete exp2;
            return false;
        } else {};
        /*第三个参数检查*/
        /*第三个参数必须是ConstantExp*/
        if (_contents.mid(thenIdx + 4).simplified() == "") {
            errorName = "Missing Parameter!";
            return false;
        } else {};
        n = new ConstantExp(_contents.mid(thenIdx + 4).simplified());
        if (!n->checkUserInputValid(errorName)){
            delete exp1;
            delete exp2;
            delete n;
            return false;
        } else {};

    }
    return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void IfStmt::normalLize(){
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
