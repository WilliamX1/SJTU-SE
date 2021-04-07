#include <cassert>
#include <string>
#include <vector>
using namespace std;

#include "multree.h"
#include "cfg.h"
#include "minisee.h"

static cfg_node* cfg;
vector<string> vartb;
vector<string> inputtb;

static cfg_node* stmt_handler(const multree_node* root, cfg_node* next);
static cfg_node* stmtlist_handler(const multree_node* root, cfg_node* next);






static bool is_arith_op(string op) {
    if( op == "+" 
    ||  op == "-"
    ||  op == "*"
    ||  op == "/"
    ||  op == "&"
    ||  op == "|"
    ||  op == "^"
    ||  op == "<<"
    ||  op == ">>"
    ||  op == "~" )
        return true;
    else
        return false;
}

static exp_node* to_boolexp(exp_node* tree) {
    assert(tree);
    if( (tree->type != exp_op) || is_arith_op(tree->val) ) {
        exp_node* zero_node = new exp_node(exp_num, "0", NULL);
        exp_node* new_node = new exp_node(exp_op, "!=", tree, zero_node);
        return new_node;
    }
    else {
        return tree;
    }
    
}

static exp_node* exp_handler(const multree_node* root) {

    if(root->child[0]->syntoken_name == "Exp") {
        exp_node* left = exp_handler(root->child[0]);
        exp_node* right = exp_handler(root->child[2]);
        return new exp_node(exp_op, root->child[1]->lexstr, left, right);
    }
    else if(root->child[0]->lextoken_name == "LP") {
        return exp_handler(root->child[1]);
    }
    else if(root->child[0]->lextoken_name == "MINUS") {
        exp_node* child = exp_handler(root->child[1]);
        return new exp_node(exp_op, "-", child);
    }
    else if(root->child[0]->lextoken_name == "NOT") {
        exp_node* child = exp_handler(root->child[1]);
        return new exp_node(exp_op, "!", child);
    }
    else if(root->child[0]->lextoken_name == "BITNOT") {
        exp_node* child = exp_handler(root->child[1]);
        return new exp_node(exp_op, "~", child);
    }
    else if(root->child[0]->lextoken_name == "ID") {
        return new exp_node(exp_var, root->child[0]->lexstr, NULL);
    }
    else if(root->child[0]->lextoken_name == "INT") {
        return new exp_node(exp_num, root->child[0]->lexstr, NULL);
    }

    assert(0);
    return NULL;
}

static cfg_node* compst_handler(const multree_node* root, cfg_node* next) {
    return stmtlist_handler(root->child[1], next);
}

static cfg_node* return_handler(const multree_node* root) {
    exp_node* tree = exp_handler(root->child[1]);
    cfg_node* node = new cfg_node(tree);

    return node;
}

static cfg_node* assert_handler(const multree_node* root, cfg_node* next) {
    exp_node* tree = exp_handler(root->child[2]);
    tree = to_boolexp(tree);
    cfg_node* node = new cfg_node(tree, next, root->lineno);

    return node;
}

static cfg_node* assign_handler(const multree_node* root, cfg_node* next) {
    exp_node* tree = exp_handler(root->child[2]);
    cfg_node* node = new cfg_node(root->child[0]->lexstr, tree, next);

    return node;
}


static cfg_node* if_handler(const multree_node* root, cfg_node* next) {
    cfg_node* true_next = stmt_handler(root->child[4], next);
    cfg_node* false_next = next;
    exp_node* tree = exp_handler(root->child[2]);
    tree = to_boolexp(tree);
    cfg_node* node = new cfg_node(tree, true_next, false_next);

    return node;
}

static cfg_node* ifelse_handler(const multree_node* root, cfg_node* next) {
    cfg_node* true_next = stmt_handler(root->child[4], next);
    cfg_node* false_next = stmt_handler(root->child[6], next);;
    exp_node* tree = exp_handler(root->child[2]);
    tree = to_boolexp(tree);
    cfg_node* node = new cfg_node(tree, true_next, false_next);

    return node;
}

static cfg_node* stmt_handler(const multree_node* root, cfg_node* next) {
    if( root->child[0]->syntoken_name == "CompSt" ) {
        return compst_handler(root->child[0], next);
    }
    else if( root->child[0]->lextoken_name == "ID" ) {
        return assign_handler(root, next);
    }
    else if( root->child[0]->lextoken_name == "RETURN" ) {
        return return_handler(root);
    }
    else if( root->child[0]->lextoken_name == "ASSERT" ) {
        return assert_handler(root, next);
    }
    else if( root->child[0]->lextoken_name == "IF" ) {
        switch( root->child_num ) {
            case 5 : return if_handler(root, next);
            case 7 : return ifelse_handler(root, next);
            default : assert(0);
        }
    }

    assert(0);
    return NULL;
}

static cfg_node* stmtlist_handler(const multree_node* root, cfg_node* next) {
    if(root->child_num == 0)
        return next;
    cfg_node* hd = stmtlist_handler(root->child[1], next);
    cfg_node* node = stmt_handler(root->child[0], hd);
    return node;
}

static void arglist_handler(const multree_node* root) {
    switch(root->child_num) {
        case 4: {
            inputtb.push_back(root->child[1]->lexstr);
            arglist_handler(root->child[3]);
            return;
        }
        case 2: {
            inputtb.push_back(root->child[1]->lexstr); 
            return;
        }
        default: assert(0); return;
    }
}

static void inputlist_handler(const multree_node* root) {
    if(root->child_num == 1)
        arglist_handler(root->child[0]);
    return;
}

static void gen_inputtb(const multree_node* root) {
    inputlist_handler(root->child[3]);
    return;
}

static void idlist_handler(const multree_node* root) {
    switch(root->child_num) {
        case 3: {
            vartb.push_back(root->child[0]->lexstr);
            idlist_handler(root->child[2]);
            return;
        }
        case 1: {
            vartb.push_back(root->child[0]->lexstr);
            return;
        }
        default: assert(0); return;
    }
    return;
}

static void gen_vartb(const multree_node* root) {
    if(root->child_num == 3)
        idlist_handler(root->child[1]);
    return;
}

static void print_tb() {
    printf("input:\n");
    unsigned num = inputtb.size();
    for(; num > 0; num --) {
        printf("%s  ", inputtb[num-1].c_str());
    }

    printf("\nvariables:\n");
    num = vartb.size();
    for(; num > 0; num --) {
        printf("%s  ", vartb[num-1].c_str());
    }

    printf("\n");

    return;
}

void print_exp(exp_node* root) {
    if(root) {
        printf("(");
        print_exp(root->child[0]);
        printf(" %s ", root->val.c_str());
        print_exp(root->child[1]);
        printf(")");
    }
}

void print_cfg(cfg_node* root) {
    if(root) {
        switch(root->type) {
            case cfg_assert: {
                printf("assert ");
                print_exp(root->exp_tree);
                printf(";\n");
                print_cfg(root->next[0]);
                break;
            }
            case cfg_assign: {
                printf("%s = ", root->dst.c_str());
                print_exp(root->exp_tree);
                printf(";\n");
                print_cfg(root->next[0]);
                break;
            }
            case cfg_return: {
                printf("return ");
                print_exp(root->exp_tree);
                printf(";\n");
                break;
            }
            case cfg_if: {
                printf("if( ");
                print_exp(root->exp_tree);
                printf(" ) {\n    ");
                print_cfg(root->next[0]);
                printf("\n} else {\n    ");
                print_cfg(root->next[1]);
                printf("\n}\n");
                break;
            }
            default: assert(0); return;
        }
    }
}

void gen_cfg(const multree_node* root) {
    gen_inputtb(root->child[0]);
    gen_vartb(root->child[2]);
    cfg = stmtlist_handler(root->child[3], NULL);

    //print_tb();
    //print_cfg(cfg);

    minisee(cfg);
    
    return;
}


