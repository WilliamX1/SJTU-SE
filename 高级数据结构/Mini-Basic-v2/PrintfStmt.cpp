#include "PrintfStmt.h"

PrintfStmt::PrintfStmt(QString stmt): Statement(stmt) {
    _contents = stmt.section(' ', 2, -1).simplified();
};
/*override*/
void PrintfStmt::printToScreen(QTextBrowser* ui_textbrowser) {
    /*打印行号*/
    ui_textbrowser->insertPlainText(QString::number(Statement::_lineNumber, 10) + " ");
    /*打印命令*/
    ui_textbrowser->insertPlainText(Statement::_cmdStr + " ");
    /*打印内容*/
    ui_textbrowser->insertPlainText(_contents + "\n");
}
/*override*/
void PrintfStmt::getSyntaxTree(){

}; //获得语句与语法树
/*override*/
bool PrintfStmt::checkUserInputValid(QString& errorName) {

    QStringList list;
    QString store_contents = _contents;

    if (_contents[0] == '\'') {
        list.append(_contents.section('\'', 0, 1) + '\'');
        _contents = _contents.mid(_contents.indexOf('\'', 1)).simplified();
    }
    else if (_contents[0] == '\"') {
        list.append(_contents.section('\"', 0, 1) + '\"');
        _contents = _contents.mid(_contents.indexOf('\"', 1)).simplified();
    }
    else {
        errorName = "Error: Wrong Format!";
        return false;
    }
    _contents = _contents.mid(1, -1);

    if (_contents == "" && list[0].count("{}") == 0) {
    } else if (_contents[0] != ',') {
        errorName = "Error: Wrong Format!";
        return false;
    } else {
        _contents = _contents.mid(1, -1);
        list.append(_contents.split(','));
    }
    /*缺少参数、太多参数*/
    int arg_num = list[0].count("{}");
    if (arg_num != list.size() - 1) {
        errorName = "Error: The Number of Parameters is not Correct!";
        return false;
    } else exps.push_back(new StringExp(list[0].simplified()));

    /*根据不同的exp创建相应*/
    for (int i = 1; i < list.size(); i++) {
        list[i] = list[i].simplified();
        Expression* new_exp = nullptr;
        switch (Expression::whichExpMightBe(list[i])) {
        case Compound: new_exp = new CompoundExp(list[i]); break;
        case Constant: new_exp = new ConstantExp(list[i]); break;
        case Identifier: new_exp = new IdentifierExp(list[i]); break;
        case String: new_exp = new StringExp(list[i]); break;
        };
        if (new_exp->checkUserInputValid(errorName) == false) return false;
        else exps.push_back(new_exp);
    };
    _contents = store_contents;
    return true;
}; //检查用户输入是否合法
/*override*/
/*标准化字符串*/
void PrintfStmt::normalLize(){
    return;
}

