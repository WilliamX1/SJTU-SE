#include "RemStmt.h"

RemStmt::RemStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1);
}
/*override*/
void RemStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void RemStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool RemStmt::checkUserInputValid(QString& errorName) {
    return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void RemStmt::normalLize(){
    return;
}
