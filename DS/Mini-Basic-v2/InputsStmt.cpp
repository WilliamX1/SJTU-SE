#include "InputsStmt.h"

InputsStmt::InputsStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
}
/*override*/
void InputsStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void InputsStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool InputsStmt::checkUserInputValid(QString& errorName) {
    /*仅有一个参数且必须是Identifier*/
    exp = new IdentifierExp(_contents);
    if (!exp->checkUserInputValid(errorName)) {
        delete exp;
        return false;
    } else {};
    return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void InputsStmt::normalLize(){
    return;
}
