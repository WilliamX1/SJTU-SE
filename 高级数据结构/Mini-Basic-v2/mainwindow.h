#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEventLoop>
#include <QMainWindow>
#include "Skiplist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void load_code();
    void run_code();
    void debug_code();
    void clear_code();
    bool input_cmd(QString str = "");
private:
    const int64_t _maxExecNum = 100000; /*上限执行语句数量*/
    Ui::MainWindow *ui;
    SkipList<QString> *myCmd;
    SkipList<QString> *myExtraCmd; /*不插入程序中的命令 LET PRINT INPUT*/
    SkipList<QString> *myVar;
    QList<QTextEdit::ExtraSelection> *myHighLight; /*高亮专用*/
    QList<QPair<int, QColor>> *highlights; /*用于维护高亮的链表*/
    QEventLoop* qeventloop;
    int64_t _extraCmdCnt;
    bool checkUserInputValid(QString& cmd, Statement*& stmt, SkipList<QString> *myCmd);
    void setButtons(bool state);
};
#endif // MAINWINDOW_H
