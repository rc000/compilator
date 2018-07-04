/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 4 "def.yy" /* yacc.c:339  */

#include <string>
#include <stdio.h>
#include <sstream>
#include <stack>
#include <vector>
#include <fstream>
#include <regex>
#include<map>
#include<iostream>
#define INFILE_ERROR 1
#define OUTFILE_ERROR 2
	extern "C" int yylex();
	extern "C" int yyerror(const char *msg, ...);
	extern FILE *yyin;
	extern FILE *yyout;

	struct symbol_info {
		std::string typ;
		std::string size;
		std::string id;
		float value;
	};
	int counter = 0;
	int textVarCounter = 0;

	int numberOfLabel = 0;

	std::map<std::string, struct symbol_info> symbols_map;
	std::vector<std::string> codelines;

	struct element {
		std::string value;
		std::string type;
		std::string tmp_name;
	};
	std::stack<struct element> stos;
	std::stack<std::string> labels;
	std::stack<struct element>comp_values;
	std::string sign_compare;


	void pushToStack(std::string value, std::string type);
	void addID(std::string, std::string, std::string);
	bool checkmap(std::string);
	void stackOperation(std::string sign);
	std::string convertArrayIndexToInt(struct element elem);
	std::string idRemove(std::string value);
	template<typename T>
	std::string toString(T value)
	{
		std::stringstream sstream;
		sstream << value;
		return sstream.str();
	}
	std::string checksymb(element el)
	{
		if (el.type == "LC")
		{
			return "LC";
		}
		else if (el.type == "LD") {
			return "LD";
		}
		else
		{
			std::map<std::string, struct symbol_info>::iterator it;
			it = symbols_map.find(el.value);
			if (it != symbols_map.end())
			{
				return it->second.typ;
			}

		}
	}


	void addID(std::string tmp, std::string typ, std::string size = "0")
	{
		symbol_info a;
		a.typ = typ;
		a.id = tmp;

		a.size = size;
		a.value = 0;
		if (typ == "STR")
		{

			a.size = "";
		}
		symbols_map[tmp] = a;

	}


	bool checkmap(std::string tmp)
	{
		std::map<std::string, struct symbol_info>::iterator it;
		it = symbols_map.find(tmp);
		if (it == symbols_map.end())
		{
			return	false;
		}
		else
			return true;
	}




	std::string computeAsm(int regNum, element zm)
	{

		std::stringstream s;

		if (zm.type == "LC")
		{
			s << "li";
		}
		else
		{
			s << "l";
		}

		if (zm.type == "LD" || zm.type == "LDA")
		{


			auto it = symbols_map.find(zm.value);
			s << ".s";
			s << " $f";
			s << regNum << ", " << it->second.id;

		}
		else if (zm.type == "ID" || zm.type == "LCA")
		{
			auto it = symbols_map.find(zm.value);
			if (it != symbols_map.end())

			{
				if (it->second.typ == "LD")
				{
					s << ".s";
					s << " $f";
					s << regNum << ", " << it->second.id;
				}
				else
				{
					s << "w $t";
					s << regNum << ", " << zm.value;
				}
			}
		}
		else
			s << " $t" << regNum << ", " << zm.value;
		return s.str();
	}
	void genSymbol(float val)
	{
		static int floatcounter = 0;
		symbol_info a;
		a.typ = "LD";
		a.id = "float_var" + std::to_string(floatcounter);
		floatcounter++;
		a.value = val;
		a.size = "0";
		std::string stri = std::to_string(val);
		std::map<std::string, struct symbol_info>::iterator it;
		it = symbols_map.find(stri);
		if (it != symbols_map.end())
		{

		}
		else {
			symbols_map[stri] = a;
		}
	}


	void pushToStack(std::string value, std::string type)
	{

		value = idRemove(value);
		struct element x;
		x.value = value;
		x.type = type;
		stos.push(x);

	}
	void declareVariable(std::string value, std::string type)
	{

		if (!checkmap(value))
		{
			addID(value, type);
		}
		else {
			std::cout << "variable already declared" << std::endl;
		}
	}

	std::string idRemove(std::string value)
	{
		if (value[0] == '@')
			value.erase(0, 1);
		return value;
	}
	void InputToAssembler()
	{


		struct element el = stos.top();
		auto it = symbols_map.find(el.value);
		if (it->second.typ == "LC")
		{
			codelines.push_back("li $v0, 5");
			codelines.push_back("syscall");
			codelines.push_back("sw $v0, " + el.value);
		}
		else if (it->second.typ == "LD")
		{
			codelines.push_back("li $v0, 6");
			codelines.push_back("syscall");
			codelines.push_back("s.s $f0, " + el.value);
		}



	}
	void IfToAssembler()
	{
		std::string line1, line2, line3;
		std::string operation1 = "i";
		std::string operation2 = "i";
		struct element comp2 = comp_values.top();
		comp_values.pop();
		struct element comp1 = comp_values.top();
		comp_values.pop();
		std::map<std::string, struct symbol_info>::iterator it;
		it = symbols_map.find(comp1.value);
		if (it != symbols_map.end())
		{
			operation1 = "w";
		}
		it = symbols_map.find(comp2.value);
		if (it != symbols_map.end())
		{
			operation2 = "w";
		}
		line1 = "l" + operation1 + " $t2, " + comp1.value;
		line2 = "l" + operation2 + " $t3, " + comp2.value;
		line3 = sign_compare + " $t2, $t3, " + labels.top();
		codelines.push_back(line1);
		codelines.push_back(line2);
		codelines.push_back(line3);

	}
	void OutputToAssembler()
	{
		struct element el = stos.top();
		auto it = symbols_map.find(el.value);
		if (it->second.typ == "LC")
		{
			codelines.push_back("lw $a0, " + el.value);
			codelines.push_back("li $v0, 1");
			codelines.push_back("syscall");

		}
		else if (it->second.typ == "LD")
		{
			codelines.push_back("l.s $f12, " + it->second.id);
			codelines.push_back("li $v0, 2");
			codelines.push_back("syscall");

		}
		else if (it->second.typ == "STR")
		{
			codelines.push_back("la $a0, " + el.value);
			codelines.push_back("li $v0, 4");
			codelines.push_back("syscall");
		}
		else if (el.type == "LC")
		{
			codelines.push_back("li $a0, " + el.value);
			codelines.push_back("li $v0, 1");
			codelines.push_back("syscall");
		}
		else if (el.type == "STR")
		{
			symbol_info a;
			a.typ = "STR";
			a.id = "string_var" + std::to_string(textVarCounter);
			textVarCounter++;
			a.size = el.value;

			symbols_map[el.value] = a;

			codelines.push_back("la $a0, " + a.id);
			codelines.push_back("li $v0, 4");
			codelines.push_back("syscall");
		}


		codelines.push_back("addi $a0, $0, 0xA \n");
		codelines.push_back("addi $v0, $0, 0xB \n");
		codelines.push_back("syscall");



	}
	void create_label()
	{

		int size = labels.size();
		std::string label = "LABEL" + std::to_string(numberOfLabel);

		labels.push(label);

		numberOfLabel++;
	}
	void jump()
	{

		std::string label = labels.top() + ":";
		labels.pop();
		create_label();
		std::string label_jump = labels.top();
		std::string jump = "j " + label_jump;
		codelines.push_back(jump);
		codelines.push_back(label);

	}
	void whileToAssembler()
	{
		std::string labelAfter = labels.top() + ":";
		labels.pop();
		std::string labelJump = "j " + labels.top();
		labels.pop();
		codelines.push_back(labelJump);
		codelines.push_back(labelAfter);
	}
	void declareArray(std::string name, std::string type, std::string size)
	{

		addID(name, type, "0:" + size);
	}
	void equation(std::string name)
	{

		name = idRemove(name);
		if (checkmap(idRemove(name)))
		{

			pushToStack(name, "ID");
			struct element el = stos.top();
			std::map<std::string, struct symbol_info>::iterator it;

			it = symbols_map.find(name);

			if (it->second.typ == "STR")
			{

				stos.pop();
				struct element el2 = stos.top();
				if (el2.type == "STR")
					it->second.size = el2.value;



			}
			else
			{

				stackOperation("=");
			}
		}
	
	}
	

	void generateArrayCode(std::string id)
	{

		std::string ASMline;
		ASMline = "\n#generate array\n";
		element elem = stos.top();
		stos.pop();
		std::string typ;

		std::map<std::string, struct symbol_info>::iterator it;
		it = symbols_map.find(id);

		if (it == symbols_map.end())
		{
			typ = it->second.typ;
		}
		else
		{
			typ = elem.type;
		}
		ASMline += "la $t2, " + id + "\n";

		if (typ == "LC")
		{
			ASMline += "li $t1, " + elem.value + "\n";
			
			if (symbols_map[id].typ == "LDA")
			{
				std::string result = "_tmp" + std::to_string(counter);
				counter++;
				symbol_info res1;
				res1.id = result;
				res1.typ = "LD";
				symbols_map[result] = res1;
				ASMline += "mtc1 $t1, $f1\n";
				ASMline += "cvt.s.w $f0, $f1\n";
				ASMline += "s.s $f0, " + result + "\n";
				ASMline += "lw $t1, " + result + "\n";
			}
		}
		else if (typ == "ID")
		{
			ASMline += "lw $t1, " + elem.value + "\n";
		}
		else
		{
			std::map<std::string, struct symbol_info>::iterator itLD;
			itLD = symbols_map.find(elem.value);

			if (symbols_map[id].typ == "LCA")
			{
				ASMline += "l.s $f0, " + itLD->second.id + "\n";
				std::string result = "_tmp" + std::to_string(counter);
				counter++;
				symbol_info res1;
				res1.id = result;
				res1.typ = "LC";

				symbols_map[result] = res1;

				ASMline += "cvt.w.s $f1, $f0\n";
				ASMline += "mfc1 $t1, $f1\n";
			}
			else {
				ASMline += "lw $t1, " + itLD->second.id + "\n";
			}
		}

		elem = stos.top();
		stos.pop();

		if ((elem.type == "ID") || (elem.type == "LCA"))
		{
			ASMline += "lw $t0, " + elem.value + "\n";

		}
		else if (elem.type == "LC")
		{
			ASMline += "li $t0, " + elem.value + "\n";
		}
		else if (elem.type == "LD")
		{
			ASMline += convertArrayIndexToInt(elem);
		}

		ASMline += "mul $t0, $t0, 4\n";
		ASMline += "add $t2, $t2, $t0\n";
		ASMline += "sw $t1, ($t2)\n\n";
		codelines.push_back(ASMline);
	}
	std::string convertArrayIndexToInt(struct element elem)
	{
		std::string ASMline;
		auto it = symbols_map.find(elem.value);
		ASMline += "l.s $f0, " + it->second.id + "\n";
		ASMline += "cvt.w.s $f1, $f0\n";
		ASMline += "mfc1 $t0, $f1\n";
		return ASMline;
	}
	void generateIndexArrayCode(std::string id)
	{
		std::string ASMline;
		element elem = stos.top();
		stos.pop();
		std::string typ = symbols_map[id].typ;
		std::string result = "_tmp" + std::to_string(counter);

		element r;
		r.value = result;
		symbol_info r1;
		r1.id = result;

		ASMline += "la $t2, " + id + "\n";

		if (elem.type == "ID")
		{
			ASMline += "lw $t0, " + elem.value + "\n";
		}
		else if (elem.type == "LC")
		{
			ASMline += "li $t0, " + elem.value + "\n";
		}
		else if (elem.type == "LD")
		{
			ASMline += convertArrayIndexToInt(elem);

		}

		ASMline += "mul $t0, $t0, 4\n";
		ASMline += "add $t2, $t2, $t0\n";

		if (typ == "LCA")
		{
			ASMline += "lw $t0, ($t2)\n";
			ASMline += "sw $t0, " + result + "\n\n";
			r.type = "LCA";
			r1.typ = "LC";
		}
		else
		{
			ASMline += "l.s $f0, ($t2)\n";
			ASMline += "s.s $f0, " + result + "\n\n";
			r.type = "LDA";
			r1.typ = "LD";

		}
		counter++;

		pushToStack(r.value, r.type);
		r1.size = "0";
		symbols_map[result] = r1;
		codelines.push_back(ASMline);
	}
	void convertOperation(struct element &el1, struct element &el2)
	{
		if ((el1.type == "LC") && (el2.type == "LD"))
		{
			codelines.push_back("#converting to float");
			codelines.push_back("mtc1 $t0, $f2");
			codelines.push_back("cvt.s.w $f0, $f2");
			el1.type = "LD";
			el2.type = "LD";
		}
		else if ((el1.type == "LD") && (el2.type == "LC"))
		{
			codelines.push_back("#converting to float");
			codelines.push_back("mtc1 $t1, $f2");
			codelines.push_back("cvt.s.w $f1, $f2");
			el1.type = "LD";
			el2.type = "LD";
		}
	}
	void convert_ID(struct element &right, struct element &left)
	{

		auto it1 = symbols_map.find(left.value);
		auto it2 = symbols_map.find(right.value);
		if ((it1->second.typ == "LC") && (it2->second.typ == "LD"))
		{
			codelines.push_back("#converting to int");
			codelines.push_back("cvt.w.s $f1, $f0");
			codelines.push_back("mfc1 $t0, $f1");
		}

		else if (((it1->second.typ == "LD") && (it2->second.typ == "LC")) || ((it1->second.typ == "LD") && (right.type == "LC")))
		{
			codelines.push_back("#converting to float");
			codelines.push_back("mtc1 $t0, $f1");
			codelines.push_back("cvt.s.w $f0, $f1");
 		}



	}
	void stackOperation(std::string sign)
	{
		std::string line1, line2, line3, line4;

		std::string tmpname = "_tmp" + toString(counter);
		counter++;
		struct element zm2 = stos.top();

		stos.pop();
		struct element zm1 = stos.top();

		stos.pop();

		std::string result = tmpname + " = " + zm1.value + " " + zm2.value + sign;
		struct symbol_info symb; 
		//create temporary variable
		if (sign != "=")
		{
			symb.id = tmpname;
			if (checksymb(zm1) == "LC" && checksymb(zm2) == "LC")
			{
				symb.typ = "LC";
				symb.size = "0";
			}
			else
			{
				symb.typ = "LD";
				symb.value = 0;
				symb.size = "0";
			}

			symbols_map[tmpname] = symb;
		}

		//add temporary variable to stack
		pushToStack(tmpname, "ID");


		if (sign == "=")
		{
			line1 = computeAsm(0, zm1);
			line2 = computeAsm(1, zm2);


			codelines.push_back(line1 + "#linia1");
			convert_ID(zm1, zm2);
 

			auto it = symbols_map.find(zm2.value);

			if (it->second.typ == "LC")
				line4 = "sw $t0," + zm2.value;
			else if (it->second.typ == "LD")
				line4 = "s.s $f0," + zm2.value;

		}
		else
		{
			line1 = computeAsm(0, zm1);

			line2 = computeAsm(1, zm2);

			codelines.push_back(line1);

			codelines.push_back(line2);
			convertOperation(zm1, zm2);

			auto it = symbols_map.find(zm2.value);

			if (zm2.type == "LC")
			{
				line3 = sign + " $t0, $t0,$t1";
				line4 = "sw $t0," + tmpname;
			}
			else if (zm2.type == "ID")
			{
				line3 = sign + " $t0, $t0,$t1";
				line4 = "sw $t0," + tmpname;

			}
			else if (zm2.type == "LD")
			{
				line3 = sign + ".s $f0, $f0,$f1";
				line4 = "s.s $f0," + tmpname;
			}
			codelines.push_back(line3);

		}
		codelines.push_back(line4);


	}
	

#line 735 "def.tab.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "def.tab.hh".  */
#ifndef YY_YY_DEF_TAB_HH_INCLUDED
# define YY_YY_DEF_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    LC = 259,
    LD = 260,
    INPUT = 261,
    OUTPUT = 262,
    FL = 263,
    INT = 264,
    TEXT = 265,
    STRING = 266,
    LEQ = 267,
    EQ = 268,
    NEQ = 269,
    GEQ = 270,
    IF = 271,
    ELSE = 272,
    WHILE = 273
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 672 "def.yy" /* yacc.c:355  */

	char *text;
	int	ival;
	float   fval;

#line 800 "def.tab.cc" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DEF_TAB_HH_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 817 "def.tab.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   149

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  89

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   273

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      23,    24,    29,    27,     2,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    19,
      32,    20,    31,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    25,     2,    26,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   696,   696,   697,   701,   702,   706,   707,   711,   712,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   724,
     727,   728,   729,   732,   735,   737,   738,   739,   742,   743,
     744,   749,   750,   751,   754,   763,   769,   775,   781,   788,
     794
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "LC", "LD", "INPUT", "OUTPUT",
  "FL", "INT", "TEXT", "STRING", "LEQ", "EQ", "NEQ", "GEQ", "IF", "ELSE",
  "WHILE", "';'", "'='", "'['", "']'", "'('", "')'", "'{'", "'}'", "'+'",
  "'-'", "'*'", "'/'", "'>'", "'<'", "$accept", "wielolinia", "linia",
  "rown", "funkcja", "tab_expr", "wyr", "els", "loop", "skladnik",
  "czynnik", "compare", "sign", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    59,
      61,    91,    93,    40,    41,   123,   125,    43,    45,    42,
      47,    62,    60
};
# endif

#define YYPACT_NINF -18

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-18)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     122,     3,   -11,    -1,    63,    78,    86,     6,   -18,   108,
     -18,    -5,    15,   -18,    14,    86,    86,    86,    86,     4,
     -18,   -18,   -18,    86,   -18,    22,   -18,    27,   -18,    86,
     -18,   -18,   -18,   -18,    86,    11,    40,   -18,   -17,    94,
     119,    89,   121,   100,    86,    48,    31,    33,    86,    86,
      86,    86,    39,   -18,   -18,    43,   114,   -18,    52,   -18,
     -18,   -18,   -18,   -18,   -18,    86,    62,    70,    40,    40,
     -18,   -18,    86,   -18,   -18,   -18,    11,   122,   122,    11,
      10,    24,    60,   -18,   -18,    75,   122,    38,   -18
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       3,     0,     0,    18,     0,     0,     0,     0,     0,    28,
      29,    30,    32,     0,    10,    28,    14,    28,    15,     0,
       1,     2,     4,     5,     0,     6,    22,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    19,     8,     9,    29,     0,    31,    29,    39,
      37,    38,    40,    35,    36,     0,     0,     0,    20,    21,
      25,    26,     0,    17,    33,    16,    34,     0,     0,     7,
       0,     0,    11,    13,    23,     0,     0,     0,    12
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -18,    20,    -9,   -18,   -18,   -18,   -14,   -18,   -18,    36,
       2,    68,   -18
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    13,    56,    85,    14,    36,
      37,    46,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      31,    35,    38,    39,    40,    52,    24,    26,    28,    42,
      48,    49,    17,     1,    32,    45,     2,     3,     4,     5,
      45,     6,    18,    15,    16,    41,     7,     1,     8,    29,
       2,     3,     4,     5,    33,     6,    82,    34,    48,    49,
       7,     1,     8,    43,     2,     3,     4,     5,    44,     6,
      83,    76,    70,    71,     7,    66,     8,    67,    79,    72,
      59,    60,    61,    62,    88,    73,    19,    20,    21,    50,
      51,    31,    31,    22,    75,    48,    49,    84,    31,    63,
      64,    25,    20,    21,    68,    69,    23,    77,    22,    27,
      20,    21,    27,    55,    21,    78,    22,    80,    81,    22,
      86,    23,    47,    27,    58,    21,    87,     0,    30,    23,
      22,     1,    23,     0,     2,     3,     4,     5,    53,     6,
       0,    48,    49,    23,     7,     1,     8,     0,     2,     3,
       4,     5,     0,     6,     0,     0,    74,     0,     7,     0,
       8,    48,    49,    54,     0,    57,    48,    49,    48,    49
};

static const yytype_int8 yycheck[] =
{
       9,    15,    16,    17,    18,    22,     4,     5,     6,    23,
      27,    28,    23,     3,    19,    29,     6,     7,     8,     9,
      34,    11,    23,    20,    21,    21,    16,     3,    18,    23,
       6,     7,     8,     9,    19,    11,    26,    23,    27,    28,
      16,     3,    18,    21,     6,     7,     8,     9,    21,    11,
      26,    65,    50,    51,    16,    24,    18,    24,    72,    20,
      12,    13,    14,    15,    26,    22,     3,     4,     5,    29,
      30,    80,    81,    10,    22,    27,    28,    17,    87,    31,
      32,     3,     4,     5,    48,    49,    23,    25,    10,     3,
       4,     5,     3,     4,     5,    25,    10,    77,    78,    10,
      25,    23,    34,     3,     4,     5,    86,    -1,     0,    23,
      10,     3,    23,    -1,     6,     7,     8,     9,    24,    11,
      -1,    27,    28,    23,    16,     3,    18,    -1,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    22,    -1,    16,    -1,
      18,    27,    28,    24,    -1,    24,    27,    28,    27,    28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     6,     7,     8,     9,    11,    16,    18,    34,
      35,    36,    37,    38,    41,    20,    21,    23,    23,     3,
       4,     5,    10,    23,    43,     3,    43,     3,    43,    23,
       0,    35,    19,    19,    23,    39,    42,    43,    39,    39,
      39,    21,    39,    21,    21,    39,    44,    44,    27,    28,
      29,    30,    22,    24,    24,     4,    39,    24,     4,    12,
      13,    14,    15,    31,    32,    45,    24,    24,    42,    42,
      43,    43,    20,    22,    22,    22,    39,    25,    25,    39,
      34,    34,    26,    26,    17,    40,    25,    34,    26
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    34,    35,    35,    36,    36,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    38,
      39,    39,    39,    40,    41,    42,    42,    42,    43,    43,
      43,    43,    43,    43,    44,    45,    45,    45,    45,    45,
      45
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     3,     6,     4,     4,
       2,     7,    11,     7,     2,     2,     5,     5,     1,     4,
       3,     3,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     3,     1,     4,     3,     1,     1,     1,     1,     1,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 696 "def.yy" /* yacc.c:1646  */
    {}
#line 1957 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 3:
#line 697 "def.yy" /* yacc.c:1646  */
    {}
#line 1963 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 6:
#line 706 "def.yy" /* yacc.c:1646  */
    { equation((yyvsp[-2].text)); }
#line 1969 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 7:
#line 707 "def.yy" /* yacc.c:1646  */
    { generateArrayCode(idRemove((yyvsp[-5].text))); }
#line 1975 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 8:
#line 711 "def.yy" /* yacc.c:1646  */
    {InputToAssembler(); }
#line 1981 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 9:
#line 712 "def.yy" /* yacc.c:1646  */
    {OutputToAssembler(); }
#line 1987 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 10:
#line 713 "def.yy" /* yacc.c:1646  */
    { declareVariable(idRemove(stos.top().value),"LD"); }
#line 1993 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 11:
#line 714 "def.yy" /* yacc.c:1646  */
    {codelines.push_back(labels.top() + ":"); labels.pop(); }
#line 1999 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 12:
#line 715 "def.yy" /* yacc.c:1646  */
    {codelines.push_back(labels.top() + ":"); labels.pop(); }
#line 2005 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 13:
#line 716 "def.yy" /* yacc.c:1646  */
    {whileToAssembler(); }
#line 2011 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 14:
#line 717 "def.yy" /* yacc.c:1646  */
    { declareVariable(idRemove(stos.top().value),"LC"); }
#line 2017 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 15:
#line 718 "def.yy" /* yacc.c:1646  */
    { declareVariable(idRemove(stos.top().value),"STR"); }
#line 2023 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 719 "def.yy" /* yacc.c:1646  */
    {declareArray(idRemove((yyvsp[-3].text)), "LCA", std::to_string((yyvsp[-1].ival))); }
#line 2029 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 720 "def.yy" /* yacc.c:1646  */
    {declareArray(idRemove((yyvsp[-3].text)), "LDA", std::to_string((yyvsp[-1].ival))); }
#line 2035 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 721 "def.yy" /* yacc.c:1646  */
    {}
#line 2041 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 724 "def.yy" /* yacc.c:1646  */
    { generateIndexArrayCode(idRemove((yyvsp[-3].text))); }
#line 2047 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 727 "def.yy" /* yacc.c:1646  */
    { stackOperation("add"); }
#line 2053 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 728 "def.yy" /* yacc.c:1646  */
    { stackOperation("sub"); }
#line 2059 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 729 "def.yy" /* yacc.c:1646  */
    {}
#line 2065 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 732 "def.yy" /* yacc.c:1646  */
    { jump(); }
#line 2071 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 735 "def.yy" /* yacc.c:1646  */
    { create_label(); codelines.push_back(labels.top() + ":"); }
#line 2077 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 25:
#line 737 "def.yy" /* yacc.c:1646  */
    { stackOperation("mul"); }
#line 2083 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 738 "def.yy" /* yacc.c:1646  */
    { stackOperation("div"); }
#line 2089 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 739 "def.yy" /* yacc.c:1646  */
    {}
#line 2095 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 742 "def.yy" /* yacc.c:1646  */
    {	pushToStack(idRemove((yyvsp[0].text)),"ID"); }
#line 2101 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 743 "def.yy" /* yacc.c:1646  */
    {	pushToStack(std::to_string((yyvsp[0].ival)),"LC");}
#line 2107 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 745 "def.yy" /* yacc.c:1646  */
    {
			genSymbol((yyvsp[0].fval));
			pushToStack(std::to_string((yyvsp[0].fval)),"LD");
		}
#line 2116 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 749 "def.yy" /* yacc.c:1646  */
    {}
#line 2122 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 750 "def.yy" /* yacc.c:1646  */
    { pushToStack((yyvsp[0].text),"STR"); }
#line 2128 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 751 "def.yy" /* yacc.c:1646  */
    {generateIndexArrayCode(idRemove((yyvsp[-3].text))); }
#line 2134 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 754 "def.yy" /* yacc.c:1646  */
    {

				struct element zn = stos.top();
				comp_values.push(zn);
				create_label();
				IfToAssembler();
				}
#line 2146 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 763 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();

	comp_values.push(zn);
	sign_compare = "ble";
}
#line 2157 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 770 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bge";
	}
#line 2167 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 776 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bne";
	}
#line 2177 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 782 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();

	comp_values.push(zn);
	sign_compare = "beq";
	}
#line 2188 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 789 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bgt";
	}
#line 2198 "def.tab.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 795 "def.yy" /* yacc.c:1646  */
    {
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "blt";
	}
#line 2208 "def.tab.cc" /* yacc.c:1646  */
    break;


#line 2212 "def.tab.cc" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 802 "def.yy" /* yacc.c:1906  */



int main(int argc, char *argv[])
{


	if (argc > 1)
	{
		yyin = fopen(argv[1], "r");
		if (yyin == NULL)
		{
			printf("Blad\n");
			return INFILE_ERROR;
		}
	}
	if (argc > 2)
	{
		yyout = fopen(argv[2], "w");
		if (yyout == NULL)
		{
			printf("Blad\n");
			return OUTFILE_ERROR;
		}
	}


	yyparse();
	fprintf(yyout, "\n");
	fprintf(yyout, ".data\n");

	for (auto y : symbols_map) {

		if ((y.second.typ == "LC") || (y.second.typ == "LCA"))
		{
			fprintf(yyout, "%s: .word %s\n", y.first.c_str(), y.second.size.c_str());
		}
		else if (y.second.typ == "LDA")
		{
			fprintf(yyout, "%s: .float %s\n", y.first.c_str(), y.second.size.c_str());
		}
		else if ((y.second.typ == "LD"))
		{
			if (y.second.value > 0)
				fprintf(yyout, "%s: .float %.5f\n", y.second.id.c_str(), y.second.value);
			else
				fprintf(yyout, "%s: .float 0\n", y.second.id.c_str());
		}
		else if (y.second.typ == "STR")
		{
			fprintf(yyout, "%s: .asciiz %s\n", y.second.id.c_str(), y.second.size.c_str());
		}

	}
	fprintf(yyout, ".text\n");
	for (auto x : codelines)
	{
		fprintf(yyout, "%s\n", x.c_str());
	}

	return 0;
}


