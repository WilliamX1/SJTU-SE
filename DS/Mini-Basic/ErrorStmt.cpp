#include "ErrorStmt.h"

ErrorStmt::ErrorStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
}
/*override*/
void ErrorStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印内容*/
    ui_textbrowser->insertPlainText(stmt + '\n');
}
/*override*/
void ErrorStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool ErrorStmt::checkUserInputValid(QString& errorName) {
    return false;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void ErrorStmt::normalLize(){
    return;
}
