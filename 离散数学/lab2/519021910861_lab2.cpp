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
    int res = 0;
    int len = s.length();

    //special judge whether len == 0 || len == 1
    if (len == 0) res = 0;
    else if (len == 1) res = s[0] - '0';
    else{
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')){ // to handle "0X..." or "0x...."
            int tmp = 1;
            for(int i = len - 1; i >= 2; i--) //to skip s[0] and s[1]
            {
                if ('0' <= s[i] && s[i] <= '9')
                    res += (s[i] - '0') * tmp;
                else if ('a' <= s[i] && s[i] <= 'z')
                    res += (s[i] - 'a' + 10) * tmp;
                else if ('A' <= s[i] && s[i] <= 'Z')
                    res += (s[i] - 'A' + 10) * tmp;
                tmp *= 16;
            }
        }
        else if (s[0] == '-'){ // to handle decimal negative number
            int tmp = 1;
            for (int i = len - 1; i >= 1; i--) //to skip s[0]
            {
                res -= (s[i] - '0') * tmp;
                tmp *= 10;
            }
        }
        else { // to handle decimal positive number
            int tmp = 1;
            for (int i = len - 1; i >= 0; i--)
            {
                res += (s[i] - '0') * tmp;
                tmp *= 10;
            }
        }
    }
    return res;
}


//root是一棵表达式树
//如果root为null，就返回null
//否则，就生成一棵新的树，这棵树表示的表达式和root表示的完全一致
//但是这棵树的所有节点都是new出来的，不可以复用root中的任何节点
exp_node* copy_exp_tree(exp_node* root) {
    //your code here
    //recursion to build a new exp_tree without using any old node!
    if (root == NULL) return NULL;
    
    exp_node* copy_root_leftchild = copy_exp_tree(root->child[0]);
    exp_node* copy_root_rightchild = copy_exp_tree(root->child[1]);
    exp_node* copy_root = new exp_node(root->type, root->val, copy_root_leftchild, copy_root_rightchild);
    //print_exp(copy_root);
    return copy_root;
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
    //your code here, you may need to use update_path_cons
    
    //to create new path constraints;
    exp_node* exp_node_T = update_path_const(state, state->path_const, true, state->cfg->exp_tree);
    exp_node* exp_node_F = update_path_const(state, state->path_const, false, state->cfg->exp_tree);

    //to create new see_state, instead of change old see_state which might cause bugs
    see_state* state_T = new see_state(state->cfg->next[0], exp_node_T);
    see_state* state_F = new see_state(state->cfg->next[1], exp_node_F);
    
    //to finish new see_state's sym_store, important!!
    state_T->sym_store = state->sym_store;
    state_F->sym_store = state->sym_store;
	/*
    int len = state->sym_store.size();
    for (int i = 0; i < len; i++)
    {
        state_T->sym_store.push_back(state->sym_store.front());
        state_F->sym_store.push_back(state->sym_store.front());
        state->sym_store.pop_back();
    }
*/
    //to push new see_state into state_queue
    state_queue.push(*state_F);
    state_queue.push(*state_T);

    //print_cfg(state->cfg);
    //std::cout << std::endl;
    return;
}




