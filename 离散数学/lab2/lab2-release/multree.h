#ifndef __MUL_TREE_H_
#define __MUL_TREE_H_

#include <cstring>

class multree_node {

public:
    multree_node* child[20];
	int child_num;
	string syntoken_name;
	int lineno;
	string lextoken_name;
	string lexstr;

    multree_node () {
        memset(child, 0, 20*sizeof(multree_node*));
        child_num = 0;
        lineno = 0;
    }

};

void display_syntree(multree_node* root);
multree_node* new_lex_node(const char* lextoken_name, const char* lexstr, int lineno);
multree_node* new_syn_node(const char* syntoken_name);
void set_multree(multree_node** root_ptr, const char* syntoken_name, int child_num, ... );

#endif
