#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <QEventLoop>
#include <QLineEdit>
#include <QTextBrowser>
#include "Statement.h"
#include "SyntaxTree.h"

template <typename T>
class SkipList {
public:
    /*四联表节点*/
    struct node {
        QString key; //行号，比较大小

        T value;    //整句话
        Statement* stmt;    //语句
        SyntaxTree* synt;   //语法树
        struct node* up;
        struct node* down;
        struct node* left;
        struct node* right;

        node (QString k, T val, Statement* s = nullptr, SyntaxTree* t = nullptr)
        : key(k), value(val), stmt(s), synt(t), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {};
    };
    T tag; //查询不到时返回值
    QString input = ""; //用户输入
    int64_t current_line_num;  //当前执行行数
private:
    /*头尾节点，不放值*/
    node* head;
    node* tail;
    int lvl_num; //当前链表层数
    /*比较大小函数*/
    bool compare(const QString A, const QString B);
    /*获取元素节点*/
    node* search(const QString& key);
public:
    SkipList(T t);
    /*插入新元素*/
    bool insert(const QString& k, T val);
    /*插入新“命令”元素，重复元素无法插入*/
    bool insert(const QString& k, T val, Statement*& stmt, bool flag = false);
    /*查找元素值*/
    T get(const QString& k);
    /*查询元素是否存在*/
    bool check(const QString& k);
    /*设置元素值value*/
    bool setValue(const QString& k, T val);
    /*删除元素*/
    bool remove(const QString& k);
    /*遍历打印树形结构*/
    void traverse(QTextBrowser *ui_textbrowser);
    void printToScreen(QTextBrowser* ui_textBrowser);
    /*获得语句与语法树*/
    bool getSyntaxTree(QString& errorName, SkipList<int64_t> *&myVar);
    /*打印语句与语法树*/
    void printSyntaxTree(QTextBrowser* ui_textBrowser);
    /*预加载*/
    void preload(SyntaxTree* synt, SkipList<int64_t> *myVar);
    /*计算后缀表达式*/
    bool calculateExp(QString& errorName, SyntaxTree* synt, SkipList<int64_t> *myVar, int64_t& res, bool isIfThen = false);
    /*执行代码*/
    bool execute(QString& errorName, SkipList<int64_t> *myVar, QTextBrowser *ui_textbrowser_terminal,
                 QTextBrowser* ui_textbrowser_result, QTextBrowser* ui_textbrowser_state, QLineEdit* ui_lineedit, QEventLoop& qeventloop, int64_t maxExecNum, bool isExtraCmd = false);
    /*打印变量*/
    void printState(QTextBrowser *ui_textBrowser);
};
#include "Skiplist.cpp"
#endif // SKIPLIST_H

