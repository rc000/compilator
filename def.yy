 
//

%{
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
	%}
%union
{
	char *text;
	int	ival;
	float   fval;
};
%type <text> wyr
%token <text> ID
%token <ival> LC
%token <fval> LD
%token INPUT
%token OUTPUT
%token FL
%token INT
%token <text> TEXT
%token <text> STRING
%token LEQ
%token EQ
%token NEQ
%token GEQ
%token <text> IF
%token <text> ELSE
%token <text> WHILE
%%
wielolinia
	:wielolinia linia{}
	| linia{}
;

linia
	:rown ';'
	| funkcja ';'
	;

rown
	:ID '=' wyr{ equation($1); }
	| ID '[' wyr ']' '=' wyr{ generateArrayCode(idRemove($1)); }
;

funkcja
	:INPUT '('wyr')' 		{InputToAssembler(); }
	| OUTPUT '('wyr')'		{OutputToAssembler(); }
	| FL czynnik{ declareVariable(idRemove(stos.top().value),"LD"); }
	| IF '('compare')' '{' wielolinia '}'		{codelines.push_back(labels.top() + ":"); labels.pop(); }
	| IF '('compare')' '{' wielolinia '}' els '{' wielolinia '}'{codelines.push_back(labels.top() + ":"); labels.pop(); }
	| loop '('compare')' '{' wielolinia '}'	{whileToAssembler(); }
	| INT czynnik{ declareVariable(idRemove(stos.top().value),"LC"); }
	| STRING czynnik{ declareVariable(idRemove(stos.top().value),"STR"); }
	| INT ID '[' LC ']'					{declareArray(idRemove($2), "LCA", std::to_string($4)); }
	| FL ID '[' LC ']'					{declareArray(idRemove($2), "LDA", std::to_string($4)); }
	| tab_expr{}
;
tab_expr
	:ID '[' wyr ']' { generateIndexArrayCode(idRemove($1)); }
;
wyr
	:wyr '+' skladnik{ stackOperation("add"); }
	| wyr '-' skladnik{ stackOperation("sub"); }
	| skladnik{}
;
els
	:ELSE{ jump(); }
;
loop
	:WHILE{ create_label(); codelines.push_back(labels.top() + ":"); }
skladnik
	:skladnik '*' czynnik{ stackOperation("mul"); }
	| skladnik '/' czynnik{ stackOperation("div"); }
	| czynnik{}
;
czynnik
	:ID{	pushToStack(idRemove($1),"ID"); }
	| LC{	pushToStack(std::to_string($1),"LC");}
	| LD
		{
			genSymbol($1);
			pushToStack(std::to_string($1),"LD");
		}
	| '(' wyr ')'		{}
	| TEXT{ pushToStack($1,"STR"); }
	| ID '[' wyr ']' 	{generateIndexArrayCode(idRemove($1)); }
;
compare
	:wyr sign wyr{

				struct element zn = stos.top();
				comp_values.push(zn);
				create_label();
				IfToAssembler();
				}
;
sign
	:'>'					{
	struct element zn = stos.top();

	comp_values.push(zn);
	sign_compare = "ble";
}
| '<'					
	{
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bge";
	}
| EQ
	{
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bne";
	}	
| NEQ
	{
	struct element zn = stos.top();

	comp_values.push(zn);
	sign_compare = "beq";
	}
| LEQ
	{
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "bgt";
	}
| GEQ
	{
	struct element zn = stos.top();
	comp_values.push(zn);
	sign_compare = "blt";
	}

;
%%


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


