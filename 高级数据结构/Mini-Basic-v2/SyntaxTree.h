#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <list>
#include <queue>
#include <stack>
#include <QString>
#include <QTextBrowser>
#include <iostream>

/*元素类型*/
enum SyntaxType{
    Imm, Var, OpPlus, OpMinus, OpMultiply, OpDivide,
    OpPower, OpLeftBracket, OpRightBracket, OpEqual,
    OpGreater, OpLess, OpTHEN, ConstStr
};
class SyntaxTree{
public:
    struct node{
        QString val;
        SyntaxType type;
        /*操作符优先级
         * 等号优先级为0
         * 加减优先级为1
         * 乘除优先级为2
         * 乘方优先级为3
        */
        int priority;
        node(QString s);
    };
private:
    QString _contents;
    /*节点个数*/
    int _size;
    /*树节点*/
    struct treenode: node{
        treenode* left;
        treenode* right;
        SyntaxType complexType;
        treenode(treenode* l = nullptr, treenode* r = nullptr, QString s = "", SyntaxType t = OpPlus)
            : node(s), left(l), right(r), complexType(t){};
    };
    treenode* head;
public:
    SyntaxTree(QString str);
    ~SyntaxTree();
    /*转换得到的后缀表达式，节点版本*/
    std::stack<node*>* _res;
    /*复制后缀表达式*/
    std::list<node*>* copyRes();
    /*设置头结点*/
    void setHead(const QString& str);
    /*标准化表达式，即每个 操作符/运算符 和 立即数/变量 之间有且仅有1个空格*/
    void normalLize(QString& str);
    /*转换成后缀表达式*/
    bool convertToPostfixExp(QString& errorName);
    /*得到表达式树*/
    bool getSyntaxTree(QString& errorName);
    /*打印表达式树*/
    void printSyntaxTree(QTextBrowser *ui_textbrowser);
};

#endif // SYNTAXTREE_H
