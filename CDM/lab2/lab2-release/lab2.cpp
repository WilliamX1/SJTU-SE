#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include"z3++.h"
using std::string;
using std::vector;
using std::map;
using std::stack;
using namespace z3;

#include "multree.h"
#include "cfg.h"
#include "minisee.h"


//下面两个print函数或许能帮你debug
//输出程序流图root对应的代码
void print_cfg(cfg_node* root);
//输出表达式树root存储的表达式
void print_exp(exp_node* root);

extern stack<see_state> state_queue;


//把字符串表示的数字转成int类型带符号整数
//s可能是10进制无符号整数、10进制带符号整数或16进制整数
int mystoi(string s) {

    //your code here
    return 1;
}


//root是一棵表达式树
//如果root为null，就返回null
//否则，就生成一棵新的树，这棵树表示的表达式和root表示的完全一致
//但是这棵树的所有节点都是new出来的，不可以复用root中的任何节点
exp_node* copy_exp_tree(exp_node* root) {

    //your code here
    return NULL;
}


//这个函数用于处理if语句
//state是if语句对应的符号执行节点
//path_const是state里面的path constraint
//condition表示if条件表达式
//branch是true时返回if条件为真对应的新的path constraint
//branch是false时返回if条件为假对应的新的path constraint
exp_node* update_path_const(see_state* state, exp_node* path_const, bool branch, exp_node* condition) {
    
    //old_const是和state里面的path constraint一样的表达式
    exp_node* old_const = copy_exp_tree(path_const);
    //cur_const是当前if条件对应的表达式
    exp_node* cur_const = update_exp_tree(state, condition);

    //exp是新的path constraint
    exp_node* exp = NULL;
    if(old_const) {
        //如果当前state里面的path constraint不是true
        exp = new exp_node(exp_op, "&&", old_const);
        if(branch) {
            exp->child[1] = cur_const;
        }
        else {
            exp->child[1] = new exp_node(exp_op, "!", cur_const);
        }
    }
    else {
        if(branch) {
            exp = cur_const;
        }
        else {
            exp = new exp_node(exp_op, "!", cur_const);
        }
    }

    return exp;
}


//处理if语句
//state中的cfg第一条语句就是一个if语句
void analyze_if(see_state* state) {
    //your code here, you may need to use update_path_const
    //cfg_node* root_T = state->cfg;
    //cfg_node* root_F = state->cfg;

    //state->cfg = root->next[0];
    cfg_node* cfg_node_T = new cfg_node(copy_exp_tree(state->path_const), state->cfg->next[0], NULL);
    see_state* state_T = new see_state(cfg_node_T, update_path_const(state, state->path_const, true, state->cfg->exp_tree));

    //state->cfg = root->next[1];
    //see_state* state_F = new see_state(state->cfg->next[1], update_path_const(state, state->path_const, false, state->cfg->exp_tree));

    state_queue.push(*state_T);
    state_queue.push(*state_F);
    return;
}




