%{
#include <stdlib.h>
#include <string.h>
#include "def.tab.hh"
extern int yylineno;
 int yyerror(const char *msg,...);
%}
%%
"Int" 		{return INT;}
"Fl"		{return FL;}
"String"	{return STRING;}
"Read"		{return INPUT;}
"Print"		{return OUTPUT;}
"If"		{return IF;}
"Else"		{return ELSE;}
"While"		{return WHILE;}
\<\=								{return LEQ;}
\=\>								{return GEQ;}
\=\=								{return EQ;}
\!\=								{return NEQ;}

\;								{return ';';}
\<								{return '<';}
\>								{return '>';}
\+			{return '+';} 
\*			{return '*';}
\(			{return '(';}
\)			{return ')';}
\/			{return '/';}
\-			{return '-';}
\=			{return '=';}
\{			{return '{';}
\}			{return '}';}
\[			{return '[';}
\]			{return ']';}



[1-9][0-9]*[.][0-9]+|0[.][0-9]+ {yylval.fval = atof(yytext);return LD;}
[1-9][0-9]*|0			{yylval.ival = atoi(yytext);return LC;}	
[@][A-Za-z_][A-Za-z0-9_]*		{yylval.text = strdup(yytext);return ID;}
\".*\"				{yylval.text = strdup(yytext); return TEXT;}
\n									{yylineno++ ;}

%%
int yyerror(const char *msg,...)
{
	printf("%d: %s", yylineno, msg);
	exit(1);
}
