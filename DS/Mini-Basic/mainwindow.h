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
    void clear_code();
    bool input_cmd(QString str = "");
private:
    /*上限执行语句数量*/
    const int64_t _maxExecNum = 100000;
    Ui::MainWindow *ui;
    SkipList<QString> *myCmd;
    /*不插入程序中的命令 LET PRINT INPUT*/
    SkipList<QString> *myExtraCmd;
    SkipList<int64_t> *myVar;
    QEventLoop qeventloop;
    int64_t _extraCmdCnt;
    bool checkUserInputValid(QString& cmd, Statement*& stmt, SkipList<QString> *myCmd);
};
#endif // MAINWINDOW_H
