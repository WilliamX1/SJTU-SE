#ifndef _CFG_H_
#define _CFG_H_

//cfg_assign表示赋值语句
//cfg_if表示if语句
//cfg_assert表示assert语句
//cfg_return表示return语句
enum CFG_NODE_TYPE { cfg_assign, cfg_if, cfg_assert, cfg_return};

//exp_var表示表达式中的变量，如a
//exp_num表示表达式中的int常数，如11
//exp_op表示表达式中的操作符，如+，-，==，>等，也包括一元操作符～等
enum EXP_NODE_TYPE { exp_var, exp_num, exp_op };


//表达式树中的节点
//可以表示布尔表达式如a>3
//也可以表示int表达式如a+b或者3
class exp_node {

public:
    //当前节点存储的数据类型，包括变量、常数和操作符
    EXP_NODE_TYPE type;
    //当前节点存储的数据
    //如果是变量就是"a"这样的变量名
    //如果是操作符就是“+”这样的操作符
    //如果是常数就是“11”这样的常数
    string val;
    //表达式树的左右节点
    exp_node* child[2];

    exp_node () {
        type = exp_var;
        child[0] = NULL;
        child[1] = NULL;
    }

    exp_node(EXP_NODE_TYPE arg_type, string arg_val, exp_node* left_child) {
        type = arg_type;
        val = arg_val;
        child[0] = left_child;
        child[1] = NULL;
    }

    exp_node(EXP_NODE_TYPE arg_type, string arg_val, exp_node* left_child, exp_node* right_child) {
        type = arg_type;
        val = arg_val;
        child[0] = left_child;
        child[1] = right_child;
    }
};


//控制流图中的节点，每个节点存一条语句
class cfg_node {

public:
    //语句类型，包括赋值、assert、if、return
    CFG_NODE_TYPE type;

    //赋值语句中等号左边的变量名
    //如a = b+c，dst就是"a"
    //如果是其他类型的语句则是空
    string dst;

    //如果是赋值语句，exp_tree存储等号右边的表达式
    //如果是return语句，exp_tree存储返回值对应的表达式
    //如果是if语句，exp_tree存储if条件对应的布尔类型表达式
    //如果是assert语句，exp_tree存储assert的参数对应的表达式
    exp_node* exp_tree;
    //这条语句在源文件中的行号，在lab中用不到
    int lineno;

    //接下来要执行的语句
    //如果当前节点是assert、return、赋值语句，那么next[0]指向下一条语句，next[1]是null
    //如果当前节点是if语句，那么next[0]指向if条件为真要执行的指令，next[1]指向if条件为假要执行的下一条指令
    //如果next[0]和next[1]都是null，说明没有下一条语句了
    cfg_node* next[2];


    cfg_node() {
        type = cfg_assign;
        exp_tree = NULL;
        lineno = 0;
        next[0] = NULL;
        next[1] = NULL;

        return;
    }

    // 赋值语句构造函数
    cfg_node(string arg_dst, exp_node* arg_tree, cfg_node* arg_next) {
        type = cfg_assign;
        dst = arg_dst;
        exp_tree = arg_tree;
        next[0] = arg_next;
        next[1] = NULL;
        lineno = 0;

        return;
    }

    // assert语句构造函数
    cfg_node(exp_node* arg_tree, cfg_node* arg_next, int arg_lineno) {
        type = cfg_assert;
        exp_tree = arg_tree;
        next[0] = arg_next;
        next[1] = NULL;
        lineno = arg_lineno;

        return;
    }

    // return语句构造函数
    cfg_node(exp_node* arg_tree) {
        type = cfg_return;
        exp_tree = arg_tree;
        next[0] = NULL;
        next[1] = NULL;
        lineno = 0;

        return;
    }

    // if语句构造函数
    cfg_node(exp_node* arg_tree, cfg_node* true_next, cfg_node* false_next) {
        type = cfg_if;
        exp_tree = arg_tree;
        next[0] = true_next;
        next[1] = false_next;
        lineno = 0;

        return;
    }

};

//存储所有局部变量的名字
extern vector<string> vartb;
//存储所有参数的名字
extern vector<string> inputtb;

void gen_cfg(const multree_node* root);
void print_cfg(cfg_node* root);
void print_exp(exp_node* root);


#endif
