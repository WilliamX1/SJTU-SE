#include "Skiplist.h"
#include "ErrorStmt.h"

/*比较大小函数*/
template <typename T>
bool SkipList<T>::compare(const QString A, const QString B) {
    if (A.length() == B.length())
        return A <= B;
    return A.length() < B.length();
}
/*获取元素节点*/
template <typename T>
typename SkipList<T>::node* SkipList<T>::search(const QString& key) {
    node* cursor = head;
    while(cursor->right != nullptr && cursor->down != nullptr) {
        if (cursor->left == nullptr) cursor = cursor->right;
        while(cursor->right != nullptr && compare(cursor->key, key))
            cursor = cursor->right;
        cursor = cursor->left;
        cursor = cursor->down;
    }
    /*寻找最后一层*/
    if (cursor->left == nullptr) cursor = cursor->right;
    while(cursor->right != nullptr && compare(cursor->key, key)) cursor = cursor->right;
    /*返回小于等于带查找值的节点*/
    return cursor->left;
};
template <typename T>
SkipList<T>::SkipList(T t): lvl_num(1), tag(t) {
    /*头尾节点内容表示层数，最底层是1*/
    if (typeid (T) == typeid (int64_t)) {
        head = new node("head", tag);
        tail = new node("tail", tag);
    } else if (typeid (T) == typeid (QString)) {
        head = new node("head", tag);
        tail = new node("tail", tag);
    }
    head->right = tail;
    tail->left = head;
}
/*插入新元素*/
template <typename T>
bool SkipList<T>::insert(const QString& k, T val) {
    /*寻找底层待插入节点*/
    node* cursor = search(k);
    /*如果元素重复则无法插入*/
    if (cursor->left != nullptr && cursor->key == k) return false;
    /*向底层插入节点*/
    node* new_node = new node(k, val);
    new_node->left = cursor, new_node->right = cursor->right; //设置新节点前驱后继
    new_node->left->right = new_node, new_node->right->left = new_node; //设置新节点的前驱节点的后继和后继节点的前驱
    /*随机是否向上生长一层*/
    while (rand() % 5 != 1) {
        /*向前移动找到第一个有上节点的节点*/
        while (cursor->left != nullptr && cursor->up == nullptr) cursor = cursor->left;
        /*当前层数不够，则需开辟新层*/
        if (cursor->up == nullptr) {
            lvl_num++;
            node* new_head = head->up = new node("head", tag);
            node* new_tail = tail->up = new node("tail", tag);

            new_head->down = head, head->up = new_head;
            new_tail->down = tail, tail->up = new_tail;
            tail = new_head->right = new_tail;
            head = new_tail->left = new_head;
        }
        /*生长一层*/
        cursor = cursor->up;
        node* _new_node = new node(k, val);
        _new_node->left = cursor, _new_node->right = cursor->right; //设置新节点前驱后继
        _new_node->left->right = _new_node, _new_node->right->left = _new_node; //设置新节点的前驱节点的后继和后继节点的前驱
        /*连通上下节点*/
        _new_node->down = new_node, new_node->up = _new_node;
        /*更新最"底层"的插入节点*/
        new_node = _new_node, new_node = _new_node;
    }
    return true;
};
/*插入新“命令”元素，重复元素无法插入*/
template <typename T>
bool SkipList<T>::insert(const QString& k, T val, Statement*& stmt, bool flag) { //flag代表是否允许重复元素
    /*寻找底层待插入节点*/
    node* cursor = search(k);
    /*如果元素重复则无法插入*/
    if (cursor->left != nullptr && cursor->key == k && !flag) return false;
    /*向底层插入节点*/
    node* new_node = new node(k, val, stmt);
    new_node->left = cursor, new_node->right = cursor->right; //设置新节点前驱后继
    new_node->left->right = new_node, new_node->right->left = new_node; //设置新节点的前驱节点的后继和后继节点的前驱
    /*随机是否向上生长一层*/
    while (rand() % 5 != 1) {
        /*向前移动找到第一个有上节点的节点*/
        while (cursor->left != nullptr && cursor->up == nullptr) cursor = cursor->left;
        /*当前层数不够，则需开辟新层*/
        if (cursor->up == nullptr) {
            lvl_num++;
            node* new_head = head->up = new node(tag, " ");
            node* new_tail = tail->up = new node(tag, " ");

            new_head->down = head, head->up = new_head;
            new_tail->down = tail, tail->up = new_tail;
            tail = new_head->right = new_tail;
            head = new_tail->left = new_head;
        }
        /*生长一层*/
        cursor = cursor->up;
        node* _new_node = new node(k, val);
        _new_node->left = cursor, _new_node->right = cursor->right; //设置新节点前驱后继
        _new_node->left->right = _new_node, _new_node->right->left = _new_node; //设置新节点的前驱节点的后继和后继节点的前驱
        /*连通上下节点*/
        _new_node->down = new_node, new_node->up = _new_node;
        /*更新最"底层"的插入节点*/
        new_node = _new_node, new_node = _new_node;
    }
    return true;
};
/*查找元素值*/
template <typename T>
T SkipList<T>::get(const QString& k) {
    node* cursor = search(k);
    if (cursor->left == nullptr || cursor->key != k) return tag;
    else return cursor->value;
};
/*查询元素是否存在*/
template <typename T>
bool SkipList<T>::check(const QString& k){
    return search(k)->left != nullptr && search(k)->key == k;
}
/*设置元素值value*/
template <typename T>
bool SkipList<T>::setValue(const QString& k, T val) {
    node* cursor = search(k);
    if (cursor->left == nullptr || cursor->key != k) return false;
    else {
        while(cursor != nullptr) {
            cursor->value = val;
            cursor = cursor->up;
        };
    };
    return true;
}
/*删除元素*/
template <typename T>
bool SkipList<T>::remove(const QString& k) {
    node* cursor = search(k);
    /*如果查无此值*/
    if (cursor->left == nullptr || cursor->key != k) return false;
    else {
        node* up_cursor = cursor;

        while (cursor != nullptr) {
            up_cursor = up_cursor->up;
            /*分别更新左右节点的后继和前驱*/
            cursor->left->right = cursor->right;
            cursor->right->left = cursor->left;
            /*删除该节点*/
            delete cursor;

            cursor = up_cursor;
        }
        /*如果该层只有一个有效节点，则删除头尾哨兵*/
        node* tmp_head = head;
        node* tmp_tail = tail;
        cursor = head;
        while (cursor->right == tail) {
            /*如果是第一层，则跳出*/
            if (cursor->down == nullptr) break;
            /*删除该层头尾哨兵*/
            head = head->down, tail = tail->down;

            cursor = cursor->down;
            delete tmp_head, delete tmp_tail;
            /*迭代*/
            tmp_head = head, tmp_tail = tail;
            tmp_head->up = tmp_tail->up = nullptr;
        }
        return true;
    }
};
/*遍历打印树形结构*/
template <typename T>
void SkipList<T>::traverse(QTextBrowser *ui_textbrowser){
    node* cursor = head;
    while(cursor != nullptr) {
        node* cur = cursor;
        while(cur != nullptr) {
            ui_textbrowser->insertPlainText(cur->key);
            cur = cur->right;
        }
        ui_textbrowser->append("");
        cursor = cursor->down;
    }
    ui_textbrowser->append("");
    ui_textbrowser->append("");
    return;
}
template <typename T>
void SkipList<T>::printToScreen(QTextBrowser* ui_textBrowser) {
    /*先清空代码框内所有代码*/
    ui_textBrowser->clear();
    /*找到最底层头节点*/
    node* cursor = head;
    while(cursor->down != nullptr) cursor = cursor->down;
    /*打印最底层*/
    cursor = cursor->right;
    while (cursor->right != nullptr) {
        cursor->stmt->printToScreen(ui_textBrowser); //ui_textBrowser->append(cursor->value);
        cursor = cursor->right;
    };
}
/*获得语句与语法树*/
template <typename T>
bool SkipList<T>::getSyntaxTree(QString& errorName, SkipList<int64_t> *&myVar) {
    /*找到最底层头节点*/
    node* cursor = head;
    while(cursor->down != nullptr) cursor = cursor->down;
    /*使用最底层结点*/
    cursor = cursor->right;
    while (cursor->right != nullptr) {
        /*只有最底层才生成语法树，节约空间*/
        cursor->synt = new SyntaxTree(cursor->value.section(' ', 2, -1).simplified());
        switch (cursor->stmt->_cmd) {
        case LET: case PRINT: case IF:
            /*分布进行转换成后缀表达式和表达式树*/
            if (!cursor->synt->convertToPostfixExp(errorName)) { cursor->stmt = new ErrorStmt(cursor->stmt->stmt); cursor->stmt->_cmd = Error; cursor->synt->setHead("Error"); break;}
            if (!cursor->synt->getSyntaxTree(errorName)) { cursor->stmt = new ErrorStmt(cursor->stmt->stmt); cursor->stmt->_cmd = Error; cursor->synt->setHead("Error"); break;}
            /*预加载时存储变量*/
            if (cursor->stmt->_cmd == LET) this->preload(cursor->synt, myVar);
            break;
        case REM: case INPUT: case GOTO: case END:
            /*不需要生成对应的语法树*/
            cursor->synt->setHead(cursor->value.section(' ', 2, -1));
            break;
        case Error:
            cursor->stmt = new ErrorStmt(cursor->stmt->stmt);
            cursor->synt->setHead("Error");
            break;
        }
        cursor = cursor->right;
    };

    return true;
}
/*打印语句与语法树*/
template <typename T>
void SkipList<T>::printSyntaxTree(QTextBrowser* ui_textBrowser) {
    /*先清空代码框内所有代码*/
    ui_textBrowser->clear();
    /*找到最底层头节点*/
    node* cursor = head;
    while(cursor->down != nullptr) cursor = cursor->down;
    /*打印最底层*/
    cursor = cursor->right;
    while (cursor->right != nullptr) {
        QString s = cursor->key;
        /*根据命令类型来生成对应语法树*/
        cmdCases cc = cursor->stmt->_cmd;
        switch (cc) {
        case REM:
            s = s + " REM";
            break;
        case LET:
            s = s + " LET";
            break;
        case PRINT:
            s = s + " PRINT";
            break;
        case INPUT:
            s = s + " INPUT";
            break;
        case GOTO:
            s = s + " GOTO";
            break;
        case IF:
            s = s + " IF";
            break;
        case END:
            s = s + " END";
            break;
        case Error:
            ui_textBrowser->append(s + " Error");
            break;
        }
        if (cc != Error) {
            /*打印命令中第一行*/
            ui_textBrowser->append(s);
            /*打印表达式树*/
            cursor->synt->printSyntaxTree(ui_textBrowser);
        }
        cursor = cursor->right;
    };

    return;
}
/*预加载*/
template <typename T>
void SkipList<T>::preload(SyntaxTree* synt, SkipList<int64_t> *myVar){
    std::list<SyntaxTree::node*>* cursor = synt->copyRes();
    /*将变量预加载进变量跳表*/
    while (!cursor->empty()) {
        if (cursor->back()->type == Var){
            myVar->insert(cursor->back()->val, INT64_MIN);
        };
        cursor->pop_back();
    };
    return;
};
/*计算后缀表达式*/
template <typename T>
bool SkipList<T>::calculateExp(QString& errorName, SyntaxTree* synt, SkipList<int64_t> *myVar, int64_t& res, bool isIfThen){
    std::list<SyntaxTree::node*>* cursor = synt->copyRes();
    std::stack<SyntaxTree::node*>* tmp = new std::stack<SyntaxTree::node*>;
    SyntaxTree::node *elemL, *elemR;
    int64_t numL, numR, numRes;
    bool flag = false;
    while (!cursor->empty()) {
        switch (cursor->back()->type) {
        /*如果是立即数则直接压栈*/
        case Imm: case Var: tmp->push(cursor->back()); break;
        /*如果是操作符则从数字栈中取出两个值，注意先取出的是右操作数*/
        case OpPlus: case OpMinus: case OpMultiply: case OpDivide: case OpPower: case OpEqual:
        case OpGreater: case OpLess: case OpTHEN:
            if (tmp->empty()) {
                errorName = "Error: Missing right-hand arithmetic number";
                return false;
            } else {
                elemR = tmp->top();
                tmp->pop();
                if (tmp->empty()) {
                    errorName = "Error: Missing left-hand arithmetic number";
                    return false;
                } else {
                    elemL = tmp->top();
                    tmp->pop();
                };
            };
            /*对立即数或变量进行转换*/
            if (myVar->get(elemR->val) != myVar->tag) {
                numR = myVar->get(elemR->val);
            } else {
                numR = elemR->val.toInt(&flag);
                if (!flag) {
                    errorName = "Error: Unrecognized variable!";
                    return false;
                } else {};
            }
            /*如果是等号则跳过对左边值的处理*/
            if (cursor->back()->type == OpEqual && !isIfThen) break;

            if (myVar->get(elemL->val) != myVar->tag) {
                numL = myVar->get(elemL->val);
            } else {
                numL = elemL->val.toInt(&flag);
                if (!flag) {
                    errorName = "Error: Unrecognized variable!";
                    return false;
                } else {};
            }
            break;
        /*如果是其他符号则出问题了*/
        default:
            errorName = "Error: Incorrect arithmetic operator!";
            break;
        }
        switch (cursor->back()->type) {
        case OpPlus:
            numRes = numL + numR;
            /*判断加法溢出*/
            if ((numL > 0 && numR > 0 && numRes < 0)
                    || (numL < 0 && numR < 0 && numRes > 0)) errorName = "Warning: Add overflow!";
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpMinus:
            numRes = numL - numR;
            /*判断减法溢出*/
            if ((numL > 0 && numR < 0 && numRes < 0)
                    || (numL < 0 && numR > 0 && numRes > 0)) errorName = "Warning: Minus Overflow!";
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpMultiply:
            numRes = numL * numR;
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpDivide:
            numRes = numL / numR;
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpPower:
            numRes = pow(numL, numR);
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpEqual:
            if (isIfThen) {
                numRes = numL == numR;
                tmp->push(new SyntaxTree::node(QString::number(numRes)));
            } else {
                if (myVar->setValue(elemL->val, numR) == false) {
                    errorName = "Error: Unrecognized variable!";
                    return false;
                } else {
                    return tmp->empty();
                };
            }
            break;
        case OpGreater:
            numRes = numL > numR;
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpLess:
            numRes = numL < numR;
            tmp->push(new SyntaxTree::node(QString::number(numRes)));
            break;
        case OpTHEN:
            numRes = numL;
            if (!tmp->empty()) return false;
            else {
                /*如果符合条件则行号为正否则为负*/
                res = numRes > 0 ? numR : -numR;
                return true;
            }
        default: break;
        };
        cursor->pop_back();
    };
    /*设置结果*/
    if (tmp->empty()) {
        errorName = "Error: Missing something!";
        return false;
    } else {
        bool flag = false;
        res = tmp->top()->val.toInt(&flag);
        if (!flag) {
            res = myVar->get(tmp->top()->val);
            if (res == myVar->tag) {
                errorName = "Error: Unrecognized variable!";
                return false;
            } else {};
        }
        tmp->pop();
        if (!tmp->empty()) {
            errorName = "Error: Extra something!";
            return false;
        }
    }
    return true;
};
/*执行代码*/
template <typename T>
bool SkipList<T>::execute(QString& errorName, SkipList<int64_t> *myVar, QTextBrowser *ui_textbrowser_terminal,
             QTextBrowser* ui_textbrowser_result, QTextBrowser* ui_textbrowser_state, QLineEdit* ui_lineedit, QEventLoop& qeventloop, int64_t maxExecNum, bool isExtraCmd){
    int64_t res = 0;
    /*找到最底层头节点*/
    node* cursor = head;
    while(cursor->down != nullptr) cursor = cursor->down;
    cursor = cursor->right;

    /*如果是不带行号命令，则只执行最新一条*/
    if (isExtraCmd){
        while (cursor->right != nullptr && cursor->right->right != nullptr)
            cursor = cursor->right;
    } else {};
    /*使用最底层结点*/
    /*当前执行已经指令数量*/
    /*如果程序为空*/
    if (cursor->right == nullptr) {
        errorName = "Warning: There is no code!";
        return false;
    }
    myVar->current_line_num = 0;
    while (cursor->right != nullptr && myVar->current_line_num <= maxExecNum) {
        bool gotoNextLine = true;
        switch (cursor->stmt->_cmd) {
        case REM: break;
        case LET:
        {
            if (!calculateExp(errorName, cursor->synt, myVar, res)) return false;
            else break;
        }
        case PRINT:
        {
            if (!calculateExp(errorName, cursor->synt, myVar, res)) return false;
            else {
                ui_textbrowser_result->append(QString::number(res));
                break;
            };
        }
        case INPUT:
        {
            if (!myVar->check(cursor->stmt->getTheFirstElem())) {
                //errorName = "Error: Unrecognized variable!";
                myVar->insert(cursor->stmt->getTheFirstElem(), INT64_MIN);
                //return false;
            }

            /*等待接收用户输入*/
            do{
                ui_textbrowser_terminal->clear();
                ui_textbrowser_terminal->append(cursor->key + " : Please input " + cursor->stmt->getTheFirstElem() + " :");
                ui_lineedit->setText("? ");
                /*设置输入焦点*/
                ui_lineedit->setFocus();
                input = QString::number(qeventloop.exec());
                if (input.toInt() != INT32_MIN) break;
            }while(true);
            /*设置值*/
            myVar->setValue(cursor->stmt->getTheFirstElem(), input.toInt());
            break;
        }
        case IF: if (!calculateExp(errorName, cursor->synt, myVar, res, true)) return false;
            else {
                if (res > 0) {
                    QString lineNumber = QString::number(res);
                    cursor = this->search(lineNumber);
                    /*防止继续下一位*/
                    gotoNextLine = false;
                    if (cursor->left == nullptr || cursor->key != lineNumber){
                        errorName = "Error: This line does not exist!";
                        return false;
                    };
                    break;
                } else {
                    break;
                }
            }
        case GOTO:
        {
            QString lineNumber = cursor->stmt->getTheFirstElem();
            cursor = this->search(lineNumber);
            /*防止继续下一位*/
            gotoNextLine = false;
            if (cursor->left == nullptr || cursor->key != lineNumber){
                errorName = "Error: This line does not exist!";
                return false;
            }
            break;
        }
        case END: {
            return true;
        }
        case Error: {
            errorName = "Error: The program stops at line " + cursor->key;
            return false;
        }
        }
        if (gotoNextLine) cursor = cursor->right;
        /*指令数增加*/
        myVar->current_line_num++;
        /*变量框更新*/
        myVar->printState(ui_textbrowser_state);
    };

    if (myVar->current_line_num > maxExecNum) {
        errorName = "Error: This program may have too many instructions or have a endless loop!";
        return false;
    } else return true;
}
template <typename T>
void SkipList<T>::printState(QTextBrowser* ui_textBrowser){
    /*先清空状态框内所有状态*/
    ui_textBrowser->clear();
    /*找到最底层头节点*/
    node* cursor = head;
    while(cursor->down != nullptr) cursor = cursor->down;
    /*打印最底层*/
    cursor = cursor->right;
    while (cursor->right != nullptr) {
        ui_textBrowser->append(cursor->key + " " + QString::number(this->get(cursor->key)));
        cursor = cursor->right;
    };
}
