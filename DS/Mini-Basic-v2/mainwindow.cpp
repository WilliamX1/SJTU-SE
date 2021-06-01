#include <QObject>
#include <QInputDialog>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Statement.h"
#include "EndStmt.h"
#include "ErrorStmt.h"
#include "Expression.h"
#include "GotoStmt.h"
#include "IfStmt.h"
#include "InputStmt.h"
#include "InputsStmt.h"
#include "LetStmt.h"
#include "PrintStmt.h"
#include "PrintfStmt.h"
#include "RemStmt.h"
#include <QMessageBox>
QString __input__; //用户输入
QString debug_line = "0"; //调试行数

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{  
    ui->setupUi(this);
    /*关联ui和动作*/
    connect(ui->button_load, SIGNAL(clicked()), this, SLOT(load_code()));
    connect(ui->button_run, SIGNAL(clicked()), this, SLOT(run_code()));
    connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(clear_code()));
    connect(ui->button_debug, SIGNAL(clicked()), this, SLOT(debug_code()));
    connect(ui->lineEdit_cmd, SIGNAL(returnPressed()), this, SLOT(input_cmd()));
    /*设置用户输入焦点*/
    ui->lineEdit_cmd->setFocus();
    /*初始化*/
    myCmd = new SkipList<QString>("TAG");
    myExtraCmd = new SkipList<QString>("TAG");
    myVar = new SkipList<QString>("TAG");
    myHighLight = new QList<QTextEdit::ExtraSelection>();
    highlights = new QList<QPair<int, QColor>>();
    qeventloop = new QEventLoop();

    _extraCmdCnt = 0;
}

MainWindow::~MainWindow()
{
    delete  ui;
    if (myCmd) delete myCmd;
    if (myExtraCmd) delete myExtraCmd;
    if (myVar) delete myVar;
    if (myHighLight) delete myHighLight;
    if (highlights) delete highlights;
    if (qeventloop) delete qeventloop;
}

void MainWindow::setButtons(bool state){
    ui->button_load->setEnabled(state);
    ui->button_run->setEnabled(state);
    ui->button_debug->setEnabled(state);
    ui->button_clear->setEnabled(state);
}
/*从文件下载代码*/
void MainWindow::load_code(){
    clear_code();
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr(" 选择导入文件"),
            "C:\\Users\\14475\\Desktop\\coding",
            tr("text files(*.txt)")
                );
    if (fileName.isEmpty()) //如果未选择文件就确认，则返回
        return;

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /*清除代码框内原有内容*/
        ui->textBrowser_code->clear();

        int line_idx = 0;
        while (!file.atEnd())
        {
            /*读取一行内容*/
            QByteArray line = file.readLine();
            /*判断是否为空行*/
            int idx = 0;
            while (line[idx] == ' ' || line[idx] == '\n' || line[idx] == '\0') idx++;
            if (idx > line.length()) continue;

            /*行数++*/
            line_idx++;
            /*调用input函数*/
            if (!input_cmd(QString(line))) {
                ui->textBrowser_terminal->append(line);
                ui->textBrowser_terminal->append("Something wrong with the " + QString::number(line_idx) + "th line!");
            }
        }
        file.close();
    }
    ui->lineEdit_cmd->clear();
}
/*运行代码*/
void MainWindow::run_code(){
    ui->textBrowser_terminal->clear();

    setButtons(false);

    if (debug_line == "0") {
        ui->textBrowser_result->clear();
    }

    QString errorName = "";
    if (debug_line.toInt() > 0) {
        while (myCmd->execute(errorName, myVar, ui->textBrowser_code, myHighLight, highlights, ui->textBrowser_terminal, ui->textBrowser_result, ui->textBrowser_state, ui->lineEdit_cmd, qeventloop, _maxExecNum, debug_line, false));
        if (errorName != "")
        {
                debug_line = "0";
                ui->textBrowser_terminal->append(errorName);
                ui->button_load->setEnabled(true);
                ui->button_clear->setEnabled(true);
        }
    } else {
        QString tmp = "-1";
        myCmd->execute(errorName, myVar, ui->textBrowser_code, myHighLight, highlights, ui->textBrowser_terminal, ui->textBrowser_result, ui->textBrowser_state, ui->lineEdit_cmd, qeventloop, _maxExecNum, tmp, false);
    }
    ui->lineEdit_cmd->clear();
    myCmd->isDebug = false;
    setButtons(true);
    return;
}
/*调试代码*/
void MainWindow::debug_code(){
    if (qeventloop->isRunning()) return;

    myCmd->isDebug = true;
    if (debug_line == "0") {
        ui->textBrowser_result->clear();
    }
    ui->textBrowser_terminal->clear();
    ui->button_load->setEnabled(false);
    ui->button_clear->setEnabled(false);
    ui->textBrowser_sentence->clear(); //语法树只需要输出当前行
    QString errorName = "";
    if (!myCmd->debug(errorName, myVar, ui->textBrowser_code, myHighLight, highlights, ui->textBrowser_terminal, ui->textBrowser_result, ui->textBrowser_state, ui->lineEdit_cmd, qeventloop, _maxExecNum, debug_line, false)) {
        ui->textBrowser_terminal->append(errorName);
        if (errorName == "INFO: This program run successfully!") {
            ui->button_load->setEnabled(true);
            ui->button_clear->setEnabled(true);
            myCmd->isDebug = false;
            debug_line = "0";
            myCmd->printSyntaxTree(ui->textBrowser_sentence, ui->textBrowser_code, myHighLight, highlights, debug_line);
            myCmd->printToScreen(ui->textBrowser_code, myHighLight, highlights);
            return;
        } else if (errorName.section(' ', 0, 0) == "Error:") {
            ui->button_load->setEnabled(true);
            ui->button_clear->setEnabled(true);
            myCmd->isDebug = false;
            debug_line = "0";
            myCmd->printSyntaxTree(ui->textBrowser_sentence, ui->textBrowser_code, myHighLight, highlights, debug_line);
            myCmd->printToScreen(ui->textBrowser_code, myHighLight, highlights);
            return;
        }
    }
    else {
        ui->textBrowser_terminal->append("The program is debuging!");
        myCmd->printSyntaxTree(ui->textBrowser_sentence, ui->textBrowser_code, myHighLight, highlights, debug_line);
    };

    ui->lineEdit_cmd->clear();
    return;
}
/*清除代码*/
void MainWindow::clear_code(){
    /*清空代码框内容*/
    ui->textBrowser_code->clear();
    ui->textBrowser_result->clear();
    ui->textBrowser_sentence->clear();
    ui->textBrowser_terminal->clear();
    ui->textBrowser_state->clear();
    /*删除原有数据*/
    delete myCmd; myCmd = nullptr;
    delete myVar; myVar = nullptr;
    delete myExtraCmd; myExtraCmd = nullptr;
    delete myHighLight; myHighLight = nullptr;
    delete highlights; highlights = nullptr;
    delete qeventloop; qeventloop = nullptr;
    /*设置用户输入焦点*/
    ui->lineEdit_cmd->setFocus();
    /*初始化*/
    myCmd = new SkipList<QString>("TAG");
    myExtraCmd = new SkipList<QString>("TAG");
    myVar = new SkipList<QString>("TAG");
    myHighLight = new QList<QTextEdit::ExtraSelection>();
    highlights = new QList<QPair<int, QColor>>();
    qeventloop = new QEventLoop();
    _extraCmdCnt = 0;
}
bool MainWindow::input_cmd(QString str){
    /*清空终端*/
    ui->textBrowser_terminal->clear();
    /*判断进程中用户输入事件*/
    if (qeventloop->isRunning()) {
        /*注意去掉头部问号*/
        __input__ = ui->lineEdit_cmd->text().section(' ', 1, -1);
        ui->lineEdit_cmd->clear();
        ui->textBrowser_terminal->clear();
        qeventloop->exit();
        return true;
    }

    /*接收用户输入*/
    QString cmd = "";
    if (str == "")
        cmd = ui->lineEdit_cmd->text();
    else
        cmd = str;

    /*去掉头尾空格回车及中间多于的空格*/
    cmd = cmd.simplified();
    /*检查是否为空*/
    if (cmd == "") {
        ui->textBrowser_terminal->append("Error: Input null!");
        return false;
    }
    /* 控制 Mini Basic 的命令 */
    if (cmd.toUpper() == "RUN") {
        cmd = cmd.toUpper();
        run_code();
        return true;
    } else if (cmd.toUpper() == "LOAD") {
        cmd = cmd.toUpper();
        load_code();
        return true;
    } else if (cmd.toUpper() == "CLEAR") {
        cmd = cmd.toUpper();
        clear_code();
        return true;
    } else if (cmd.toUpper() == "HELP") {
        cmd = cmd.toUpper();
        QMessageBox* Q = new QMessageBox(this);
        Q->setWindowTitle("帮助");

        QString info = "Mini Basic can only execute no more than " + QString::number(_maxExecNum) + " instructions!";
        QString REM_HELP =   "REM\t--REM\t\t\t--注释";
        QString LET_HELP =   "LET\t--LET var = exp\t\t--赋值";
        QString PRINT_HELP = "PRINT\t--PRINT exp\t\t--打印输出";
        QString INPUT_HELP = "INPUT\t--INPUT var\t\t--用户输入";
        QString GOTO_HELP =  "GOTO\t--GOTO n\t\t--跳转";
        QString IF_HELP =    "IF\t--IF exp1 op exp2 THEN n\n\t\t\t\t--条件判断";
        QString END_HELP =   "END\t--END\t\t\t--终止";
        QString RUN = "RUN\t--RUN\t\t\t--运行";
        QString LOAD = "LOAD\t--LOAD\t\t\t--从文件中导入";
        QString CLEAR = "CLEAR\t--CLEAR\t\t--清空";
        QString HELP = "HELP\t--HELP\t\t\t--帮助";
        QString TOTLE = info + '\n' + REM_HELP + '\n' + LET_HELP + '\n' + PRINT_HELP + '\n' + INPUT_HELP + '\n' +
                GOTO_HELP + '\n' + IF_HELP + '\n' + END_HELP + '\n' + RUN + '\n' + LOAD + '\n' + CLEAR + '\n' + HELP;

        Q->setText(TOTLE);
        Q->exec();
        ui->lineEdit_cmd->clear();
        return true;
    } else if (cmd.toUpper() == "QUIT"){
        cmd = cmd.toUpper();
        this->close();
        return true;
    }

    /*检查是否为不带行号的PRINT、LET、INPUT指令*/
    if (cmd.section(' ', 0, 0).toUpper() == "PRINT"
            || cmd.section(' ', 0, 0).toUpper() == "PRINTF"
            || cmd.section(' ', 0, 0).toUpper() == "LET"
            || cmd.section(' ', 0, 0).toUpper() == "INPUT"
            || cmd.section(' ', 0, 0).toUpper() == "INPUTS") {
        /*将命令大写化*/
        cmd = cmd.section(' ', 0, 0).toUpper() + ' ' + cmd.section(' ', 1, -1);
        /*加上行号*/
        cmd = (QString::number(_extraCmdCnt) + ' ' + cmd).simplified();
        /*行号增加*/
        _extraCmdCnt++;

        Statement* stmt = nullptr;
        /*检查并保存用户输入*/
        if (!checkUserInputValid(cmd, stmt, myExtraCmd)) stmt->_cmd = Error;
        /*保存在额外指令中*/
        myExtraCmd->insert(cmd.section(' ', 0, 0), cmd, stmt);
        /*生成语句与语法树并发送*/
        QString errorName = "";
        if (myExtraCmd->getSyntaxTree(errorName, myVar) == false)
        {
            ui->textBrowser_terminal->insertPlainText("ErrorType: Syntax wrong!\t");
            ui->textBrowser_terminal->insertPlainText(errorName);
            /*重置用户输出框为空*/
            ui->lineEdit_cmd->clear();
            /*删除跳表中存储的命令结点*/
            myExtraCmd->remove(cmd.section(' ', 0, 0));
            return false;
        } else {};
        /*重置用户输出框和终断框为空*/
        ui->lineEdit_cmd->clear();
        ui->textBrowser_terminal->clear();

        QString tmp = "-1";
        if (!myExtraCmd->execute(errorName, myVar, ui->textBrowser_code, myHighLight, highlights, ui->textBrowser_terminal, ui->textBrowser_result, ui->textBrowser_state, ui->lineEdit_cmd, qeventloop, _maxExecNum, tmp, true)) {
            ui->textBrowser_terminal->append(errorName);
            return false;
        } else {
            ui->textBrowser_terminal->append("The single statement run successfully!");
        };
        ui->lineEdit_cmd->clear();
        /*重绘状态框*/
        myVar->printState(ui->textBrowser_state);
        return true;
    }

    /*检查是否带行号*/
    bool _have_line_number = false;
    if (cmd.section(' ', 0, 0).toInt(&_have_line_number) > 0 && _have_line_number) {}
    else {
        ui->textBrowser_terminal->append("Error: No line number!");
        ui->lineEdit_cmd->clear();
        return false;
    }

    Statement* stmt = nullptr;
    /*检查并保存用户输入*/
    if (!checkUserInputValid(cmd, stmt, myCmd)) stmt->_cmd = Error;
    if (cmd != "DELETE ONE LINE") myCmd->insert(cmd.section(' ', 0, 0), cmd, stmt);
    /*生成语句与语法树并发送*/
    QString errorName = "";
    myCmd->getSyntaxTree(errorName, myVar);

    /*将用户输入发送到语句与语法树框*/
    myCmd->printSyntaxTree(ui->textBrowser_sentence, ui->textBrowser_code, myHighLight, highlights, "-1");
    /*将用户输入发送到代码提示框*/
    myCmd->printToScreen(ui->textBrowser_code, myHighLight, highlights);
    /*重置用户输出框为空*/
    ui->lineEdit_cmd->clear();

    return true;
}
bool MainWindow::checkUserInputValid(QString& cmd, Statement*& stmt, SkipList<QString> *myCmd) {
    QString lineNumber = cmd.section(' ', 0, 0);
    QString cmdtype = cmd.section(' ', 1, 1);
    /*清除上一条命令错误信息*/
    ui->textBrowser_terminal->clear();

    /*行号非法判定*/
    bool flag = false;
    lineNumber.toInt(&flag);
    if (!flag) {
        ui->textBrowser_terminal->setText("Error: LineNumber is invalid!  Not an interger or Too large");
        stmt = new ErrorStmt(cmd);
        return false;
    } else if (lineNumber.toInt() < 0) {
        ui->textBrowser_terminal->setText("Error: LineNumber cannot be negative!");
        stmt = new ErrorStmt(cmd);
        return false;
    } else if (lineNumber.toInt() > 1000000) {
        ui->textBrowser_terminal->setText("Error: LineNumber cannot exceed 100000!");
        stmt = new ErrorStmt(cmd);
        return false;
    } else if (myCmd->get(lineNumber) != myCmd->tag && cmdtype.toUpper() != "") {
        ui->textBrowser_terminal->setText("Error: This line already exists!");
        stmt = new ErrorStmt(cmd);
        return false;
    }

    /*如果使用小写则警告*/
    if (cmdtype != cmdtype.toUpper()) {
        ui->textBrowser_terminal->setText("Warning: Lower case will be converted to upper case automatically!");
        cmdtype = cmdtype.toUpper();
        /*转换成大写*/
        cmd = lineNumber + ' ' + cmdtype + ' ' + cmd.section(' ', 2, -1);
    }
    /*根据命令类型创建相应对象*/
    if (cmdtype.toUpper() == "REM")
        stmt = new RemStmt(cmd);
    else if (cmdtype.toUpper() == "LET")
        stmt = new LetStmt(cmd);
    else if (cmdtype.toUpper() == "PRINT")
        stmt = new PrintStmt(cmd);
    else if (cmdtype.toUpper() == "PRINTF")
        stmt = new PrintfStmt(cmd);
    else if (cmdtype.toUpper() == "INPUT")
        stmt = new InputStmt(cmd);
    else if (cmdtype.toUpper() == "INPUTS")
        stmt = new InputsStmt(cmd);
    else if (cmdtype.toUpper() == "GOTO")
        stmt = new GotoStmt(cmd);
    else if (cmdtype.toUpper() == "IF")
        stmt = new IfStmt(cmd);
    else if (cmdtype.toUpper() == "END")
        stmt = new EndStmt(cmd);
    /*清除某行代码*/
    else if (cmdtype.toUpper() == "")
    {
        if (myCmd->remove(lineNumber)) {
            ui->textBrowser_terminal->setText("Information: Remove line " + lineNumber + " successfully!");
            /*将用户输入发送到语句与语法树框*/
            myCmd->printSyntaxTree(ui->textBrowser_sentence, ui->textBrowser_code, myHighLight, highlights, "-1");
            /*将用户输入发送到代码提示框*/
            myCmd->printToScreen(ui->textBrowser_code, myHighLight, highlights);
        } else ui->textBrowser_terminal->setText("Information: " + lineNumber + " does not exists!");
        cmd = "DELETE ONE LINE";
        return true;
    }
    else {
        ui->textBrowser_terminal->setText("Error: Command type is invalid!");
        stmt = new ErrorStmt(cmd);
        return false;
    }

    /*语法判定*/
    QString errorName = "";
    /*先对语句进行标准化*/
    stmt->normalLize();
    if (!stmt->checkUserInputValid(errorName)) {
        ui->textBrowser_terminal->insertPlainText("ErrorType: Syntax wrong!\t");
        ui->textBrowser_terminal->insertPlainText(errorName);
        stmt = new ErrorStmt(cmd);
        return false;
    } else {
        return true;
    }
}
