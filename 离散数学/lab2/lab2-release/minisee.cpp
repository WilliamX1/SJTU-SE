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


//函数参数的名字
extern vector<string> inputtb;
//局部变量的名字
extern vector<string> vartb;
//用于深度优先遍历符号执行树的栈
stack<see_state> state_queue;
//symbolic store是一个数组，每个变量对应其中一个元素
//name_to_index存储了从变量名字到变量在symbolic store里面下标的映射
//lab2中不涉及对name_to_index的操作
static map<string, int> name_to_index;


exp_node* update_exp_tree(see_state* state, exp_node* exp) {
    if(exp == NULL)
        return NULL;

    exp_node* new_exp = NULL;
    if(exp->type == exp_var) {
        new_exp = copy_exp_tree( state->sym_store[ name_to_index[exp->val] ] );
        assert(new_exp);
    }
    else {
        new_exp = new exp_node;
        new_exp->type = exp->type;
        new_exp->val = exp->val;
        new_exp->child[0] = update_exp_tree(state, exp->child[0]);
        new_exp->child[1] = update_exp_tree(state, exp->child[1]);
    }

    return new_exp;

}


//把state的symbolic store里变量dst的值改成exp表示的表达式
void update_sym_storage(string dst, see_state* state, exp_node* exp) {
    assert(exp);

    exp_node* new_exp = update_exp_tree(state, exp);
    state->sym_store[ name_to_index[dst] ] = new_exp;
    return;
}


//处理赋值语句
//当前state里面的cfg第一条指令是一条赋值语句
void analyze_assign(see_state* state) {
    //root指向一条赋值语句
    cfg_node* root = state->cfg;
    //更新state里面的next code为赋值后面的语句
    state->cfg = root->next[0];

    //root->dst是被赋值的变量
    //root->exp_tree是表达式树，存储赋值语句右边的表达式
    //例如a = b + 3
    //root->dst就是a，exp_tree里面表示的就是b+3
    //update_sym_storage是更新state里面的symbolic store
    //把symbolic store里面a的值改成b+3
    update_sym_storage( root->dst, state, root->exp_tree );

    //符号执行树中的下一个节点压入栈中，等待执行
    state_queue.push(*state);

    return;
}


//把用表达式树表示的合式公式exp_tree转成Z3的expr类型，也就是Z3认识的合式公式
//要求exp_tree不为空
expr gen_fol(context* cxt, const expr_vector e, exp_node* exp_tree) {
    assert(exp_tree);

    switch(exp_tree->type) {
        case exp_var: {
            return e[ name_to_index[exp_tree->val] ];
        }
        case exp_num: {
            return cxt->bv_val( mystoi(exp_tree->val), 32);
        }
        case exp_op: {
            if(exp_tree->val == ">")
                return gen_fol(cxt, e, exp_tree->child[0]) > gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "<")
                return gen_fol(cxt, e, exp_tree->child[0]) < gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == ">=")
                return gen_fol(cxt, e, exp_tree->child[0]) >= gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "<=")
                return gen_fol(cxt, e, exp_tree->child[0]) <= gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "==")
                return gen_fol(cxt, e, exp_tree->child[0]) == gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "!=")
                return gen_fol(cxt, e, exp_tree->child[0]) != gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "+")
                return gen_fol(cxt, e, exp_tree->child[0]) + gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "-") {
                if(exp_tree->child[1])
                    return gen_fol(cxt, e, exp_tree->child[0]) - gen_fol(cxt, e, exp_tree->child[1]);
                else
                    return -gen_fol(cxt, e, exp_tree->child[0]);
            }
            else if(exp_tree->val == "*")
                return gen_fol(cxt, e, exp_tree->child[0]) * gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "/")
                return gen_fol(cxt, e, exp_tree->child[0]) / gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "&&")
                return gen_fol(cxt, e, exp_tree->child[0]) && gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "||")
                return gen_fol(cxt, e, exp_tree->child[0]) || gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "!")
                return !gen_fol(cxt, e, exp_tree->child[0]);
            else if(exp_tree->val == "&")
                return gen_fol(cxt, e, exp_tree->child[0]) & gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "|")
                return gen_fol(cxt, e, exp_tree->child[0]) | gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "^")
                return gen_fol(cxt, e, exp_tree->child[0]) ^ gen_fol(cxt, e, exp_tree->child[1]);
            else if(exp_tree->val == "<<")
                return shl(gen_fol(cxt, e, exp_tree->child[0]), gen_fol(cxt, e, exp_tree->child[1]));
            else if(exp_tree->val == ">>")
                return ashr(gen_fol(cxt, e, exp_tree->child[0]), gen_fol(cxt, e, exp_tree->child[1]));
            else if(exp_tree->val == "~")
                return ~ gen_fol(cxt, e, exp_tree->child[0]);
            else
                assert(0);
        }
        default: assert(0);
    }

}


//s是定义的SMT solver
//print_counterexample按照参数定义顺序输出反例
//例如函数func(int a, int b, int c)
//那么就会依次输出a, b, c三个参数的值，使得assert报错
void print_counterexample(solver s) {

    int counterexample[100];
    memset(counterexample, 0, 100*sizeof(int));

    model m = s.get_model();

    for(int i=0; i<m.size(); ++i) {
        func_decl v = m[i];
        string name = v.name().str();
        int value = mystoi( m.get_const_interp(v).get_decimal_string(10) );
        counterexample[ name_to_index[name] ] = value;
    }

    for(int i=0; i<inputtb.size(); ++i) {
        std::cout << counterexample[i] << " ";
    }

    std::cout << "\n";

    return;
}


//验证assert的条件是否永远成立
void verify(see_state* state) {
    //root指向一个assert语句
    cfg_node* root = state->cfg;
    //更新下一条要执行的语句
    state->cfg = root->next[0];

    //new_exp存储的是当前assert的条件对应的符号表达式
    exp_node* new_exp = update_exp_tree(state, root->exp_tree);
    assert(new_exp);

    //下面调用SMT库函数进行验证
    //判断“assert的条件不成立”是不是可满足的
    context cxt;
    expr_vector e(cxt);
    unsigned num = inputtb.size();
    //每个参数都对应公式中的一个带符号整数
    //因为是32位整数，所以z3中用bit vector 32类型表示(bv_const)
    for(int i = 0; i < num; ++ i)
        e.push_back( cxt.bv_const(inputtb[i].c_str(), 32) );
    
    //target是assert的条件对应的合式公式
    expr target = gen_fol( &cxt, e, new_exp );

    solver s(cxt);
    if(state->path_const) {
        //如果有path constraint的话
        //path_const是path constraint对应的合式公式
        expr path_const = gen_fol(&cxt, e, state->path_const);
        //利用SMT判断path_const -> target是否永真
        //也就是判断path_const /\ !(target)是否可满足
        s.add( path_const && (!target) );
    }
    else {
        //如果state->path_const是NULL
        //说明path_constraint是true
        //直接判断assert的条件target是否永真
        //也就是判断!target是否可满足
        s.add( !target );
    }
    switch( s.check() ) {
        case sat:
            //如果可满足，那么说明assert条件可能不成立
            //std::cout << "error!!! ";
            //按参数声明的顺序输出反例
            print_counterexample(s);
            break;
        case unsat:
            //如果不可满足，说明assert的条件永真，不会报错，程序正确
            std::cout << "verified" << std::endl;
            break;
        default:
            //如果SMT solver无法求解成功，说明出问题了。
            //本lab不存在这种情况
            std::cout << "exception" << std::endl;
    }

    state_queue.push(*state);

    return;
}


//state是当前符号执行节点的状态
//包括symbolic store, path constraint和next code
void state_handler(see_state* state) {
    //root是将要执行的代码
    cfg_node* root = state->cfg;
    
    //如果没有代码了就退出
    if(root == NULL)
        return;

    //如果有代码要执行，分类处理
    switch(root->type) {
        //代码是赋值语句
        case cfg_assign: analyze_assign(state); break;
        //代码是assert，调用verify进行验证
        case cfg_assert: verify(state); break;
        //代码是return，直接结束
        case cfg_return: break;
        //代码是if
        case cfg_if: analyze_if(state); break;
        //不可能有其它类型的代码
        default: assert(0);
    }

    return;
}


//state是符号执行树中的根节点，init_see初始化其中的状态
void init_see(see_state* state) {
    //num1是参数数量
    unsigned num1 = inputtb.size();
    //num2是局部变量的数量
    unsigned num2 = vartb.size();
    
    
    unsigned i;
    //把每个参数的初始值存入symbolic store
    for(i = 0; i < num1; ++ i) {
        //symbolic store存的每个变量的值是一个符号表达式，用表达式树表示
        //初始情况下参数x的值就是"x"
        //对应exp_node*指向的表达式树就只有一个根节点，里面存了变量x。
        state->sym_store.push_back(new exp_node(exp_var, inputtb[i], NULL));
        //在name_to_index中记录每个参数对应的exp_node*在symbolic store里的下标
        name_to_index[ inputtb[i] ] = i;
    }

    //把每个局部变量的初始值存入symbolic store
    for(i = 0; i < num2; ++ i) {
        //因为还没有执行语句，所以它们都是没有初始值的
        //在symbolic store里面存的exp_node*就是NULL
        //程序执行起来后会给这些局部变量赋值，到时候会更新symbolic store
        state->sym_store.push_back(NULL);
        //在name_to_index中记录每个局部变量对应的exp_node*在symbolic store里的下标
        name_to_index[ vartb[i] ] = i + num1;
    }

    return;
}


//从这里开始符号执行，代码是cfg
void minisee(cfg_node* cfg) {
    
    //init_state是符号执行中的根节点
    see_state* init_state = new see_state(cfg, NULL);
    //填写根节点相关内容，包括path constraint、symbolic store等
    init_see(init_state);


    //把符号执行树的根节点压栈，开始深度优先搜索
    state_queue.push(*init_state);
    while( !state_queue.empty() ) {
        //cur_state是当前符号执行到的节点
        see_state cur_state = state_queue.top();
        state_queue.pop();

        //DEBUG信息
        //print_cfg(cur_state.cfg);
        //print_exp(cur_state.path_const);
        //printf("\n---------------------------------\n");

        //处理当前符号执行节点，生成新的节点压入栈中
        //如果遇到assert就开始验证
        state_handler(&cur_state);
    }

    return;
}



