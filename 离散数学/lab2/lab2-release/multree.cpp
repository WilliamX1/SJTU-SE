#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <string>
using namespace std;
#include "multree.h"




static multree_node* new_multree_node()
{
	multree_node* new_node = new multree_node;
	assert( new_node );
    return new_node;
}

static void insert_node(multree_node* parent, multree_node* child)
{
	assert( parent->child_num < 20 );
	parent->child[parent->child_num] = child;
	parent->child_num++;
	if( child )
 	{
		if( parent->lineno==0 )
			parent->lineno = child->lineno;
	}
}

static void print_node(multree_node* root, int depth)
{
	if( root==NULL )
		return;
	int i;
	if( !(root->syntoken_name.empty()) )
	{
		for(i=0; i<10; ++i)
			if( root->child[i]!=0 )
				break;
		if( i<10 )
		{
			for( i=0; i<depth; ++i )
				printf("  ");
			printf("%s (%d)\n", root->syntoken_name.c_str(), root->lineno );
		}
	}
	else
	{ 
		for( i=0; i<depth; ++i )
			printf("  ");
		if( (root->lextoken_name == "ID") || (root->lextoken_name == "TYPE") 
		||  (root->lextoken_name == "INT") 
		)
		{
			printf("%s: %s\n", root->lextoken_name.c_str(), root->lexstr.c_str() );
		}
		else
			printf("%s\n", root->lextoken_name.c_str());
	}
	for(i=0; i<root->child_num; ++i)
		print_node(root->child[i], depth+1);
}


void display_syntree(multree_node* root)
{
    //return;
	print_node(root, 0);
}

static void record_syntoken_name( multree_node* node, const char* syntoken_name )
{
	assert( syntoken_name );
	node->syntoken_name = syntoken_name;
}

static void record_lextoken_name( multree_node* node, const char* lextoken_name )
{
    assert( lextoken_name );
	node->lextoken_name = lextoken_name;
}

static void record_lexstr( multree_node* node, const char* lexstr )
{
    assert( lexstr );
	node->lexstr = lexstr;
}

multree_node* new_lex_node(const char* lextoken_name, const char* lexstr, int lineno)
{
//	printf("\nnew %s %s \n", lextoken_name, lexstr);
	multree_node* temp = new_multree_node();
	record_lextoken_name( temp, lextoken_name );
	record_lexstr( temp, lexstr );
	temp->lineno = lineno;
	return temp;
}

multree_node* new_syn_node(const char* syntoken_name)
{
	multree_node* temp = new_multree_node();
	record_syntoken_name( temp, syntoken_name );
	return temp;
}

void set_multree(multree_node** root_ptr, const char* syntoken_name, int child_num, ... )
{
	*root_ptr = new_syn_node(syntoken_name);

	va_list arg_ptr;
	va_start( arg_ptr, child_num );
	int i;
	for( i=0; i<child_num; ++i )
	{ 
		multree_node* para = va_arg(arg_ptr, multree_node*);
		insert_node( *root_ptr, para );
	}
	va_end( arg_ptr );
}




