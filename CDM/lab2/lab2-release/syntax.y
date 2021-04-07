%{
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>
using namespace std;
#include "multree.h"
#include "cfg.h"

static int if_lexsyn_error = 0;

#include "lex.yy.c"

void yyerror(const char* msg);
%}

%error-verbose

%token INT ID
%token SEMI COMMA
%token ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR NOT BITOR BITAND XOR BITNOT SHR SHL
%token TYPE
%token LP RP LC RC
%token RETURN IF ELSE WHILE ASSERT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left BITOR
%left XOR
%left BITAND
%left RELOP
%left SHL SHR
%left PLUS MINUS
%left STAR DIV
%right NOT NEG BITNOT
%left LP RP

%%

Program : FuncDef LC DefList StmtList RC { set_multree(&$$, "Program", 5, $1, $2, $3, $4, $5); if( !if_lexsyn_error ) {/*display_syntree($$);*/ gen_cfg($$);} }
	;
FuncDef : TYPE ID LP InputList RP { set_multree(&$$, "FuncDef", 5, $1, $2, $3, $4, $5); }
        ;
InputList : ArgList { set_multree(&$$, "InputList", 1, $1); }
          | { set_multree(&$$, "InputList", 0); }
          ;
ArgList : TYPE ID COMMA ArgList { set_multree(&$$, "ArgList", 4, $1, $2, $3, $4); }
        | TYPE ID { set_multree(&$$, "ArgList", 2, $1, $2); }
        ;
IDList : ID COMMA IDList { set_multree(&$$, "IDList", 3, $1, $2, $3); }
       | ID { set_multree(&$$, "IDList", 1, $1); }
       ;
DefList : TYPE IDList SEMI { set_multree(&$$, "DefList", 3, $1, $2, $3); }
        | { set_multree(&$$, "DefList", 0); }
	;
StmtList  : Stmt StmtList { set_multree(&$$, "StmtList", 2, $1, $2); }
	  | { set_multree(&$$, "StmtList", 0); }
          ; 
Stmt : ID ASSIGNOP Exp SEMI { set_multree(&$$, "Stmt", 4, $1, $2, $3, $4); }
     | CompSt { set_multree(&$$, "Stmt", 1, $1); }
     | RETURN Exp SEMI { set_multree(&$$, "Stmt", 3, $1, $2, $3); }
     | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { set_multree(&$$, "Stmt", 5, $1, $2, $3, $4, $5); }
     | IF LP Exp RP Stmt ELSE Stmt { set_multree(&$$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
     | ASSERT LP Exp RP SEMI { set_multree(&$$, "Stmt", 5, $1, $2, $3, $4, $5); }
     ;
Exp : Exp AND Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp OR Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp XOR Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp BITOR Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp BITAND Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp RELOP Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp PLUS Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp MINUS Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp STAR Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp DIV Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp SHL Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | Exp SHR Exp { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | LP Exp RP { set_multree(&$$, "Exp", 3, $1, $2, $3); }
    | MINUS Exp %prec NEG { set_multree(&$$, "Exp", 2, $1, $2); }
    | NOT Exp { set_multree(&$$, "Exp", 2, $1, $2); }
    | BITNOT Exp { set_multree(&$$, "Exp", 2, $1, $2); }
    | ID { set_multree(&$$, "Exp", 1, $1); }
    | INT { set_multree(&$$, "Exp", 1, $1); }
    ;
CompSt : LC StmtList RC { set_multree(&$$, "CompSt", 3, $1, $2, $3); }
       ;

%%

int main(int argc, char** argv)
{
	FILE* f = fopen(argv[1], "r");
	if( !f )
	{ 
		printf("error file\n");
		return 1;
	}
	yyrestart(f);
	yyparse();
	return 0;
} 

void yyerror(const char* msg)
{ 
	if_lexsyn_error = 1; 
	fprintf(stderr, "Error at line %d: %s\n", yylineno, msg);
}
