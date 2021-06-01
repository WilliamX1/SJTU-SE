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
    case String: exp = nullptr; break;
    }

    if (!exp->checkUserInputValid(errorName)) {
        delete exp;
        return false;
    } else return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void PrintStmt::normalLize(){
    return;
}
