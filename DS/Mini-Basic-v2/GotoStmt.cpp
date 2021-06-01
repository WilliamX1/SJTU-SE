#include "GotoStmt.h"

GotoStmt::GotoStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
}
/*override*/
void GotoStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void GotoStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool GotoStmt::checkUserInputValid(QString& errorName) {
    /*仅有一个参数且必须是常量*/
    exp = new ConstantExp(_contents);
    return exp->checkUserInputValid(errorName);
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void GotoStmt::normalLize(){
    return;
}
