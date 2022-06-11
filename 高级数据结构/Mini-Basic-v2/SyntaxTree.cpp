#include "SyntaxTree.h"

SyntaxTree::node::node(QString s){
    val = s.simplified();
    bool flag = false;
    val.toInt(&flag);
    if (val == "+") { type = OpPlus; priority = 1; }
    else if (val == "-") { type = OpMinus; priority = 1; }
    else if (val == "*") { type = OpMultiply; priority = 2; }
    else if (val == "/") { type = OpDivide; priority = 2; }
    else if (val == "**") { type = OpPower; priority = 3; }
    else if (val == "=") { type = OpEqual; priority = 0; }
    else if (val == "(") { type = OpLeftBracket; priority = 0; }
    else if (val == ")") { type = OpRightBracket; priority = 0; }
    /*大于优先级很低*/
    else if (val == ">") { type = OpGreater; priority = -1; }
    else if (val == "<") { type = OpLess; priority = -1; }
    else if (val.toUpper() == "THEN") { type = OpTHEN; priority = -100; }
    else if (flag) type = Imm;
    else if (val[0] == '\"' || val[0] == '\'') type = ConstStr;
    else type = Var;
}
SyntaxTree::SyntaxTree(QString str){
    _contents = str;
    _size = 0;
    head = nullptr;
    /*标准化字符串*/
    normalLize(_contents);
};
SyntaxTree::~SyntaxTree() {
    treenode* tmp = head, *cursor = head;
    while (tmp != nullptr) {
        cursor = tmp->right;
        delete tmp;
        tmp = cursor;
    };
    head = nullptr;

    if (_res) delete _res;
    _res = nullptr;
    return;
}
/*复制后缀表达式*/
std::list<SyntaxTree::node*>* SyntaxTree::copyRes(){
    std::list<node*> *newres = new std::list<node*>;
    while (!_res->empty()) {
        /*将元素插入到链表的尾部*/
        newres->push_back(_res->top());
        _res->pop();
    };
    /*将_res弄回来*/
    std::list<node*>::iterator it = --newres->end();
    while (it != newres->begin()){
        _res->push(*it);
        it--;
    }; _res->push(*it); //头部也要push进来
    /*返回的newres头部 --> newres尾部 是后缀表达式*/
    return newres;
};
/*设置头结点*/
void SyntaxTree::setHead(const QString& str) {
    head = new treenode(nullptr, nullptr, str);
    return;
}
/*标准化表达式，即每个 操作符/运算符 和 立即数/变量 之间有且仅有1个空格*/
void SyntaxTree::normalLize(QString& _contents){
    /*防止首字母就是运算符导致数组越界*/
    _contents = " " + _contents.simplified() + " ";
    for (int i = 0; _contents[i] != '\0'; i++){
        /*特殊考虑乘方*/
        if (_contents[i] == '*' && (i + 1) <= _contents.length() && _contents[i + 1] == '*') {
            _contents = _contents.mid(0, i) + " " + _contents[i] + _contents[i + 1] + " " + _contents.mid(i + 2, -1);
            i += 3;
        }
        /*凡是运算符、括号前后均加空格*/
        else if (_contents[i] == '+' || _contents[i] == '-' || _contents[i] == '*' || _contents[i] == '/' || _contents[i] == '='
                 || _contents[i] == '>' || _contents[i] == '<' || _contents[i] == '(' || _contents[i] == ')') {
            _contents = _contents.mid(0, i) + " " + _contents[i] + " " + _contents.mid(i + 1, -1);
            i += 2;
        } else continue;
    }
    _contents = _contents.simplified();
    /*规定负数必须用括号括起来*/
    for (int i = 0; _contents[i] != '\0'; i++) {
        if (_contents[i] == '-') {
            if (i - 2 < 0 || _contents[i - 2] == '(') {
                _contents[i] = ' ';
                _contents[i + 1] = '-';
                i++;
            }
        }
    }
    _contents = _contents.simplified();
    return;
}
/*转换成后缀表达式*/
bool SyntaxTree::convertToPostfixExp(QString& errorName){
    std::stack<node*>* _saved = new std::stack<node*>();
    _res = new std::stack<node*>();

    QString tmp = _contents.simplified();
    while(tmp != "") {
        /*识别每一个并存入节点集*/
        node* newnode = nullptr;
        if (tmp[0] == '\"') {
            newnode = new node(tmp.section('\"', 0, 1).simplified() + '\"');
            tmp = tmp.section('\"', 2, -1).simplified();
        }
        else if (tmp[0] == '\'') {
            newnode = new node(tmp.section('\'', 0, 1).simplified() + '\'');
            tmp = tmp.section('\'', 2, -1).simplified();
        }
        else {
            newnode = new node(tmp.section(' ', 0, 0).simplified());
            tmp = tmp.section(' ', 1, -1).simplified();
        }

        switch (newnode->type) {
        /*如果遇到操作数或变量或字符串，则直接入后缀栈*/
        case Imm: case Var: case ConstStr: _res->push(newnode); break;
        /*如果遇到括号，则先检查暂存栈中情况，再放入后缀栈*/
        case OpLeftBracket: { _saved->push(newnode); break; }
        case OpRightBracket:
        {
            /*匹配左括号*/
            while (!_saved->empty() && _saved->top()->type != OpLeftBracket) {
                _res->push(_saved->top());
                _saved->pop();
            };
            /*如果没有匹配成功*/
            if (_saved->empty()){
                errorName = "Error: No LeftBracket!";
                return false;
            } else {
                /*弹出左括号*/
                _saved->pop();
            }
            break;
        }
        /*如果遇到运算符*/
        default:
        {
            while (!_saved->empty() && _saved->top()->priority >= newnode->priority){
                /*如果是乘方操作需要谨慎*/
                if (newnode->priority == 3) break;
                /*其他操作*/
                _res->push(_saved->top());
                _saved->pop();
            }
            _saved->push(newnode);
            break;
        }
        };

        _size++;
    }
    /*将剩余的暂存栈中内容输出*/
    while(!_saved->empty()) {
        if (_saved->top()->type == OpLeftBracket) {
            errorName = "Error: No rightBracket!";
            return false;
        }
        _res->push(_saved->top());
        _saved->pop();
    }
    return true;
}
/*得到表达式树*/
bool SyntaxTree::getSyntaxTree(QString& errorName){
    /*如果表达式为空*/
    if (_res->empty()) {
        errorName = "Error: The expression is empty!";
        return false;
    }

    std::stack<treenode*> tmp;
    std::stack<node*> save_res;
    /*将后缀表达式翻过来*/
    while(!_res->empty()) {
        tmp.push(new treenode(nullptr, nullptr, _res->top()->val, _res->top()->type));
        /*注意保护好_res之后运行时需要用到*/
        save_res.push(_res->top());
        _res->pop();
    }
    std::stack<treenode*> saved;
    /*进行转换*/
    int str_cnt = 0;
    while(!tmp.empty()){
        switch (tmp.top()->complexType) {
        /*立即数或变量或字符串则直接进暂存栈*/
        case Imm: case Var:
            saved.push(tmp.top());
            tmp.pop();
            break;
        case ConstStr:
            str_cnt++;
            if (str_cnt > 1) {
                errorName = "Error: No arithmetic operation between strings!";
                return false;
            };
            saved.push(tmp.top());
            tmp.pop();
            break;
        /*括号直接报错*/
        case OpLeftBracket: case OpRightBracket:
            errorName = "Error: Extra brackets!";
            return false;
            break;
        /*操作符*/
        case OpPlus: case OpMinus: case OpMultiply: case OpDivide: case OpPower: case OpEqual:
        case OpGreater: case OpLess: case OpTHEN:
            treenode* OpNode = tmp.top();
            tmp.pop();
            if (saved.empty()) {
                errorName = "Error: Missing operands!";
                return false;
            } else {
                OpNode->right = saved.top();
                saved.pop();
                if (saved.empty()){
                    /*处理负号*/
                    if (OpNode->complexType == OpMinus) {
                        OpNode->right->val = "-" + OpNode->right->val;
                        saved.push(OpNode->right);
                        /*及时退出*/
                        break;
                    } else {
                        errorName = "Error: Missing operands!";
                        return false;
                    }
                } else {
                    OpNode->left = saved.top();
                    saved.pop();
                };
            };
            /*该节点可以当成一个立即数*/
            OpNode->complexType = Imm;
            saved.push(OpNode);
            break;
        }
    }
    /*头结点赋值*/
    head = saved.top();
    saved.pop();
    if (!saved.empty()) {
        errorName = "Error: Extra imm or extra var!";
        return false;
    };
    /*将_res弄回来*/
    while (!save_res.empty()) {
        _res->push(save_res.top());
        save_res.pop();
    };

    return true;
};
/*打印表达式树*/
void SyntaxTree::printSyntaxTree(QTextBrowser *ui_textbrowser){
    std::queue<treenode*> q1;
    std::queue<treenode*> q2;
    int layer = 1;
    if (head == nullptr) return;

    q1.push(head);
    while(((layer & 1) && !q1.empty())
          || (!(layer & 1) && !q2.empty()))
    {
        /*遍历奇数层*/
        if (layer & 1) {
            while(!q1.empty()) {
                /*打印对应层数多的空tab*/
                QString s = "";
                for (int i = 0; i < layer; i++) s = s + "  ";
                /*打印对应元素*/
                s = s + q1.front()->val;
                ui_textbrowser->append(s);
                /*将左右子节点放入*/
                if (q1.front()->left != nullptr) q2.push(q1.front()->left);
                if (q1.front()->right != nullptr) q2.push(q1.front()->right);
                q1.pop();
            };
        } else {
            while (!q2.empty()) {
                /*打印对应层数多的空tab*/
                QString s = "";
                for (int i = 0; i < layer; i++) s = s + "  ";
                /*打印对应元素*/
                s = s + q2.front()->val;
                ui_textbrowser->append(s);
                /*将左右子节点放入*/
                if (q2.front()->left != nullptr) q1.push(q2.front()->left);
                if (q2.front()->right != nullptr) q1.push(q2.front()->right);
                q2.pop();
            }
        }
        layer++;
    };

}
