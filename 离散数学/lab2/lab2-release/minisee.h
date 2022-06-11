#ifndef _SEE_H_
#define _SEE_H_

//see_state是当前符号执行节点的状态
//包括symbolic store, path constraint和next code
class see_state {
public:
    //symbolic store，是一个数组
    //每个变量在其中有一个表达式树exp_node*
    vector<exp_node*> sym_store;
    //path constraint
    //如果是null说明path constraint是true
    exp_node* path_const;
    //控制流图control flow graph(CFG)
    //表示接下来要执行的所有代码
    //cfg->root就指向符号执行节点中的next code，就是将要执行的代码
    cfg_node* cfg;

    see_state() {
        path_const = NULL;
        cfg = NULL;
        return;
    }

    see_state(cfg_node* arg_cfg, exp_node* arg_path) {
        cfg = arg_cfg;
        path_const = arg_path;
        
        return;
    }
};

void minisee(cfg_node* cfg);

exp_node* update_exp_tree(see_state* state, exp_node* exp);
void analyze_if(see_state* state);
exp_node* copy_exp_tree(exp_node* root);
int mystoi(string s);


#endif

