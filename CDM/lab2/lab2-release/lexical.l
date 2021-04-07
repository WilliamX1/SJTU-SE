%{  
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <string>
using namespace std;

#include "multree.h"
#define YYSTYPE multree_node*
#include "syntax.tab.h"

extern "C" int yylex();
%}

%option yylineno
digit [0-9]
letter [_a-zA-Z]
decimal (0|([1-9]{digit}*))
%%

0|([1-9]{digit}*) {
					yylval = new_lex_node("INT", yytext, yylineno); 
					return INT;
				  }
0[xX][0-9a-fA-F]+ {
					yylval = new_lex_node("INT", yytext, yylineno); 
					return INT;
				  }
; {  
	yylval = new_lex_node("SEMI", yytext, yylineno);
	return SEMI;
  }
, {  
	yylval = new_lex_node("COMMA", yytext, yylineno);
	return COMMA;
  }
= {  
	yylval = new_lex_node("ASSIGNOP", yytext, yylineno);
	return ASSIGNOP;
  }
>|<|(>=)|(<=)|(==)|(!=) {  
							yylval = new_lex_node("RELOP", yytext, yylineno);
							return RELOP;
						}
\+ {  
		yylval = new_lex_node("PLUS", yytext, yylineno);
		return PLUS;
   }
- { 
		yylval = new_lex_node("MINUS", yytext, yylineno);
		return MINUS;
  } 
\* {  
		yylval = new_lex_node("STAR", yytext, yylineno);
		return STAR;
   } 
"/" {   
		yylval = new_lex_node("DIV", yytext, yylineno);
		return DIV;
	}
&& {   
		yylval = new_lex_node("AND", yytext, yylineno);
		return AND;
   }
\|\| { 
		yylval = new_lex_node("OR", yytext, yylineno);
		return OR;
	 }
! {   
		yylval = new_lex_node("NOT", yytext, yylineno);
		return NOT;
  }
& {   
		yylval = new_lex_node("BITAND", yytext, yylineno);
		return BITAND;
  }
\| {
		yylval = new_lex_node("BITOR", yytext, yylineno);
		return BITOR;
   }
\^ {
		yylval = new_lex_node("XOR", yytext, yylineno);
		return XOR;
   }
(<<) {
		yylval = new_lex_node("SHL", yytext, yylineno);
		return SHL;
   }
(>>) {
		yylval = new_lex_node("SHR", yytext, yylineno);
		return SHR;
   }
~ {
		yylval = new_lex_node("BITNOT", yytext, yylineno);
		return BITNOT;
   }
int|bool { 
		yylval = new_lex_node("TYPE", yytext, yylineno); 
		return TYPE;
	}
\( {   
		yylval = new_lex_node("LP", yytext, yylineno);
		return LP;
   }
\) {   
		yylval = new_lex_node("RP", yytext, yylineno);
		return RP;
   }
"{" {  
		yylval = new_lex_node("LC", yytext, yylineno);
		return LC;
	} 
"}" {  
		yylval = new_lex_node("RC", yytext, yylineno);
		return RC;
	}
return { 
			yylval = new_lex_node("RETURN", yytext, yylineno);
			return RETURN;
	    }
if {
		yylval = new_lex_node("IF", yytext, yylineno);
		return IF;
   }
else {   
		yylval = new_lex_node("ELSE", yytext, yylineno);
		return ELSE;
	 }
assert {   
		yylval = new_lex_node("ASSERT", yytext, yylineno);
		return ASSERT;
	 }
{letter}({digit}|{letter})* {
								yylval = new_lex_node("ID", yytext, yylineno);
								return ID;
							}

"//"[^\n]*\n {  }
"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" { }
\n|" "|\t { }

. { if_lexsyn_error = 1; printf("Error at line %d : Mysterious string %s\n", yylineno, yytext); }

%%

