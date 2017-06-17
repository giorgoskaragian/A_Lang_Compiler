%code requires {
#include "ast.h"
#include "linked_list.h"
#include "icode.h"
#include "tcode.h"
}

%{
#include "parser.hpp"
#include <stdio.h>
#include <stdarg.h> // va_list, va_start, va_arg, va_end

int yyerror(const char* msg);
int yylex();

extern int yylineno;
extern char* yytext;
extern FILE* yyin;

Program* g_Program = 0;

void printRule(int lineno, const char* parent, const char* child, ...);
%}

%error-verbose
%start program

%union 
{
	char* strVal;
	int intVal;
	double doubleVal;
	struct ASTNode* astNode;
	struct LinkedList* list;
	struct Program* prog;
}

%token <intVal> INTEGER_CONST;
%token <doubleVal> REAL_CONST;
%token <strVal> STRING_CONST;
%token <astNode> IDENTIFIER;
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL;
%token EQUAL PLUS MINUS MUL DIV MOD DOUBLE_EQUAL NOT_EQUAL INCREMENT DECREMENT GREATER_THAN LESS_THAN GREATER_THAN_OR_EQUAL LESS_THAN_OR_EQUAL;
%token OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET OPEN_BRACKET CLOSE_BRACKET OPEN_PARENTHESIS CLOSE_PARENTHESIS SEMICOLON COMMA COLON DOUBLE_COLON DOT DOUBLE_DOT;

%type <prog> program
%type <astNode> statement expression indexed_element lvalue call function_definition
%type <list> statement_list expression_list indexed_element_list identifier_list func_block

%right EQUAL
%left OR
%left AND
%nonassoc DOUBLE_EQUAL NOT_EQUAL
%nonassoc GREATER_THAN GREATER_THAN_OR_EQUAL LESS_THAN LESS_THAN_OR_EQUAL
%left PLUS MINUS
%left MUL DIV MOD
%right NOT INCREMENT DECREMENT UNARY_MINUS
%left DOT DOUBLE_DOT
%left OPEN_BRACKET CLOSE_BRACKET
%left OPEN_PARENTHESIS CLOSE_PARENTHESIS

%nonassoc THEN
%nonassoc ELSE

%%

program: statement_list
			{ 
				$$ = progCreate($1);
				g_Program = $$;
				printRule(yylineno, "PROGRAM", "STATEMENT_LIST"); 
			}
       ;

statement_list: statement_list statement 
				{ 
					llPushBack($1, $2);
					$$ = $1;

					printRule(yylineno, "STATEMENT_LIST", "STATEMENT_LIST STATEMENT"); 
				}
			  | /* empty */
				{ 
					$$ = llCreate();
					printRule(yylineno, "STATEMENT_LIST", "EMPTY");                    
				}
              ;

statement: expression SEMICOLON 
			{ 
				$$ = astCreateStatement_Expression(yylineno, $1);
				printRule(yylineno, "STATEMENT", "EXPRESSION;"); 
			}
		 | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement %prec THEN 
			{ 
				$$ = astCreateStatement_If(yylineno, $3, $5, 0);
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT"); 
			}
         | IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement ELSE statement 
			{ 
				$$ = astCreateStatement_If(yylineno, $3, $5, $7);
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT else STATEMENT"); 
			}
         | WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement 
			{ 
				$$ = astCreateStatement_While(yylineno, $3, $5);
				printRule(yylineno, "STATEMENT", "while (EXPRESSION) STATEMENT"); 
			}
         | FOR OPEN_PARENTHESIS expression_list SEMICOLON expression SEMICOLON expression_list CLOSE_PARENTHESIS statement 
			{ 
				$$ = astCreateStatement_For(yylineno, $3, $5, $7, $9);
				printRule(yylineno, "STATEMENT", "for (EXPRESSION_LIST; EXPRESSION; EXPRESSION_LIST) STATEMENT"); 
			}
         | RETURN expression SEMICOLON 
			{ 
				$$ = astCreateStatement_Return(yylineno, $2);
				printRule(yylineno, "STATEMENT", "return EXPRESSION;"); 
			}
		 | RETURN SEMICOLON            
			{ 
				$$ = astCreateStatement_Return(yylineno, 0);
				printRule(yylineno, "STATEMENT", "return;"); 
			}
         | BREAK SEMICOLON             
			{ 
				$$ = astCreateStatement_Generic(yylineno, ST_Break);
				printRule(yylineno, "STATEMENT", "break;"); 
			}
         | CONTINUE SEMICOLON          
			{ 
				$$ = astCreateStatement_Generic(yylineno, ST_Continue);
				printRule(yylineno, "STATEMENT", "continue;"); 
			}
		 | OPEN_CURLY_BRACKET statement_list CLOSE_CURLY_BRACKET                       
			{ 
				$$ = astCreateStatement_Block(yylineno, $2);
				printRule(yylineno, "STATEMENT", "{ STATEMENT_LIST }"); 
			}
         | function_definition
			{ 
				$$ = astCreateStatement_FuncDef(yylineno, $1);
				printRule(yylineno, "STATEMENT", "FUNC_DEF"); 
			}
         | SEMICOLON                   
			{ 
				$$ = astCreateStatement_Generic(yylineno, ST_Empty);
				printRule(yylineno, "STATEMENT", ";"); 
			}
         ;

expression: lvalue EQUAL expression
			{ 
				$$ = astCreateExpression_Assignment(yylineno, $1, $3);
				printRule(yylineno, "EXPRESSION", "LVALUE = EXPRESSION");       
			}
          | expression PLUS expression
			{ 
				$$ = astCreateExpression_Op(yylineno, $1, $3, Op_Add);
				printRule(yylineno, "EXPRESSION", "EXPRESSION + EXPRESSION");   
			}
          | expression MINUS expression
			{ 
				$$ = astCreateExpression_Op(yylineno, $1, $3, Op_Subtract);
				printRule(yylineno, "EXPRESSION", "EXPRESSION - EXPRESSION");   
			}
          | expression MUL expression
			{
				$$ = astCreateExpression_Op(yylineno, $1, $3, Op_Multiply);
				printRule(yylineno, "EXPRESSION", "EXPRESSION * EXPRESSION");   
			}
          | expression DIV expression
			{ 
				$$ = astCreateExpression_Op(yylineno, $1, $3, Op_Divide);
				printRule(yylineno, "EXPRESSION", "EXPRESSION / EXPRESSION");   
			}
          | expression MOD expression
			{ 
				$$ = astCreateExpression_Op(yylineno, $1, $3, Op_Modulo);
				printRule(yylineno, "EXPRESSION", "EXPRESSION % EXPRESSION");   
			}
          | expression GREATER_THAN expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_GreaterThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION > EXPRESSION");   
			}
          | expression GREATER_THAN_OR_EQUAL expression
			{
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_GreaterThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION >= EXPRESSION");
			}
          | expression LESS_THAN expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_LessThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION < EXPRESSION");   
			}
          | expression LESS_THAN_OR_EQUAL expression
			{
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_LessThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION <= EXPRESSION");
			}
          | expression DOUBLE_EQUAL expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_Equal);
				printRule(yylineno, "EXPRESSION", "EXPRESSION == EXPRESSION");  
			}
          | expression NOT_EQUAL expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_NotEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION != EXPRESSION");  
			}
          | expression AND expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_And);
				printRule(yylineno, "EXPRESSION", "EXPRESSION and EXPRESSION"); 
			}
          | expression OR expression
			{ 
				$$ = astCreateExpression_LogicOp(yylineno, $1, $3, LogicOp_Or);
				printRule(yylineno, "EXPRESSION", "EXPRESSION or EXPRESSION");  
			}
          | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
			{
				$$ = astCreateExpression_Nested(yylineno, $2);
				printRule(yylineno, "EXPRESSION", "(EXPRESSION)");
			}
          | MINUS expression %prec UNARY_MINUS
			{
				$$ = astCreateExpression_UnaryOp(yylineno, $2, UnaryOp_Minus);
				printRule(yylineno, "EXPRESSION", "-EXPRESSION");
			}
          | NOT expression
			{ 
				$$ = astCreateExpression_UnaryOp(yylineno, $2, UnaryOp_Not);
				printRule(yylineno, "EXPRESSION", "not EXPRESSION");
			}
          | INCREMENT lvalue
			{ 
				$$ = astCreateExpression_UnaryOp(yylineno, $2, UnaryOp_PreInc);
				printRule(yylineno, "EXPRESSION", "++LVALUE");
			}
          | lvalue INCREMENT
			{ 
				$$ = astCreateExpression_UnaryOp(yylineno, $1, UnaryOp_PostInc);
				printRule(yylineno, "EXPRESSION", "LVALUE++");
			}
          | DECREMENT lvalue
			{
				$$ = astCreateExpression_UnaryOp(yylineno, $2, UnaryOp_PreDec);
				printRule(yylineno, "EXPRESSION", "--LVALUE");
			}
          | lvalue DECREMENT
			{
				$$ = astCreateExpression_UnaryOp(yylineno, $1, UnaryOp_PostDec);
				printRule(yylineno, "EXPRESSION", "LVALUE--");
			}
          | lvalue
			{ 
				$$ = astCreateExpression_lValue(yylineno, $1);
				printRule(yylineno, "EXPRESSION", "LVALUE");
			}
          | call
			{ 
				$$ = astCreateExpression_Call(yylineno, $1);
				printRule(yylineno, "EXPRESSION", "CALL");
			}
          | OPEN_BRACKET expression_list CLOSE_BRACKET
			{ 
				$$ = astCreateExpression_ObjDef(yylineno, $2, ObjDef_ExpressionList);
				printRule(yylineno, "EXPRESSION", "[EXPRESSION_LIST]");
			}
		  | OPEN_BRACKET indexed_element_list CLOSE_BRACKET         
			{ 
				$$ = astCreateExpression_ObjDef(yylineno, $2, ObjDef_IndexedElementList);
				printRule(yylineno, "EXPRESSION", "[INDEXED_ELEMENT_LIST]");
			}
          | OPEN_PARENTHESIS function_definition CLOSE_PARENTHESIS  
			{
				$$ = astCreateExpression_FuncDef(yylineno, $2);
				printRule(yylineno, "EXPRESSION", "(FUNC_DEF)");
			}
          | INTEGER_CONST
			{
				$$ = astCreateExpression_ConstInt(yylineno, $1);
				printRule(yylineno, "EXPRESSION", "CONST INTEGER (%d)", $1);
			}
		  | REAL_CONST
			{
				$$ = astCreateExpression_ConstReal(yylineno, $1);
				printRule(yylineno, "EXPRESSION", "CONST REAL (%f)", $1);
			}
		  | STRING_CONST
			{
				$$ = astCreateExpression_ConstString(yylineno, $1);
				printRule(yylineno, "EXPRESSION", "CONST STRING (\"%s\")", $1);
			}
		  | NIL
			{
				$$ = astCreateExpression_ConstNil(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST nil");
			}
		  | TRUE
			{
				$$ = astCreateExpression_ConstTrue(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST true");
			}
		  | FALSE
			{
				$$ = astCreateExpression_ConstFalse(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST false");
			}
          ;

expression_list: expression_list COMMA expression   
				{
					llPushBack($1, $3);
					$$ = $1;
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION_LIST, EXPRESSION");
				}
			   | expression
				{ 
					$$ = llCreate();
					llPushBack($$, $1);
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION");
				}
               | /* empty */
				{ 
					$$ = llCreate();
					printRule(yylineno, "EXPRESSION_LIST", "EMPTY");
				}
               ;

indexed_element: OPEN_CURLY_BRACKET expression COLON expression CLOSE_CURLY_BRACKET
				{
					$$ = astCreateIndexedElement(yylineno, $2, $4);
					printRule(yylineno, "INDEXED_ELEMENT", "{ EXPRESSION:EXPRESSION }");
				}
               ;

indexed_element_list: indexed_element_list COMMA indexed_element
					{
						llPushBack($1, $3);
						$$ = $1;
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT_LIST, INDEXED_ELEMENT");
					}
                    | indexed_element
					{
						$$ = llCreate();
						llPushBack($$, $1);
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT");
					}
                    ;

lvalue: IDENTIFIER
		{
			$$ = astCreateLValue_Identifier(yylineno, $1);
			printRule(yylineno, "LVALUE", "IDENTIFIER (%s)", astGetIdentifierName($1));
		}
      | LOCAL IDENTIFIER
		{
			$$ = astCreateLValue_LocalIdentifier(yylineno, $2);
			printRule(yylineno, "LVALUE", "local IDENTIFIER (%s)", astGetIdentifierName($2));
		}
      | DOUBLE_COLON IDENTIFIER
		{
			$$ = astCreateLValue_GlobalIdentifier(yylineno, $2);
			printRule(yylineno, "LVALUE", "::IDENTIFIER (%s)", astGetIdentifierName($2));
		}
      | lvalue DOT IDENTIFIER
		{
			$$ = astCreateLValue_ObjectMember(yylineno, $1, $3);
			printRule(yylineno, "LVALUE", "LVALUE.IDENTIFIER (%s)", astGetIdentifierName($3));
		}
      | lvalue OPEN_BRACKET expression CLOSE_BRACKET  
		{
			$$ = astCreateLValue_ArrayElement(yylineno, $1, $3);
			printRule(yylineno, "LVALUE", "LVALUE[EXPRESSION]");
		}
      | call DOT IDENTIFIER
		{
			$$ = astCreateLValue_CallObjectMember(yylineno, $1, $3);
			printRule(yylineno, "LVALUE", "CALL.IDENTIFIER (%s)", astGetIdentifierName($3));
		}
      | call OPEN_BRACKET expression CLOSE_BRACKET
		{
			$$ = astCreateLValue_CallArrayElement(yylineno, $1, $3);
			printRule(yylineno, "LVALUE", "CALL[EXPRESSION]");
		}
      ;

call: call OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS 
		{
			$$ = astCreateCall_Call(yylineno, $1, $3);
			printRule(yylineno, "CALL", "CALL(EXPRESSION_LIST)");
		}
    | lvalue OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS 
		{
			$$ = astCreateCall_Normal(yylineno, $1, $3);
			printRule(yylineno, "CALL", "LVALUE(EXPRESSION_LIST)");
		}
	| lvalue DOUBLE_DOT IDENTIFIER OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS  
		{
			$$ = astCreateCall_Method(yylineno, $1, $3, $5);
			printRule(yylineno, "CALL", "LVALUE..IDENTIFIER(EXPRESSION_LIST) (%s)", astGetIdentifierName($3));
		} 
    | OPEN_PARENTHESIS function_definition CLOSE_PARENTHESIS OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS 
		{
			$$ = astCreateCall_Lambda(yylineno, $2, $5);
			printRule(yylineno, "CALL", "(FUNC_DEF)(EXPRESSION_LIST)");
		}
    ;

func_block: OPEN_CURLY_BRACKET statement_list CLOSE_CURLY_BRACKET 
		{ 
			$$ = $2;
			printRule(yylineno, "FUNC_BLOCK", "{ STATEMENT_LIST }");
		}
     ;

function_definition: FUNCTION IDENTIFIER OPEN_PARENTHESIS identifier_list CLOSE_PARENTHESIS func_block 
					{ 
						$$ = astCreateFuncDef(yylineno, $2, $4, $6);
						printRule(yylineno, "FUNC_DEF", "function IDENTIFIER (%s) (IDENTIFIER_LIST) FUNC_BLOCK", astGetIdentifierName($2));
					}
                   | FUNCTION OPEN_PARENTHESIS identifier_list CLOSE_PARENTHESIS func_block 
					{
						$$ = astCreateFuncDef(yylineno, 0, $3, $5);
						printRule(yylineno, "FUNC_DEF", "function (IDENTIFIER_LIST) FUNC_BLOCK");
					}
                   ;

identifier_list: identifier_list COMMA IDENTIFIER
				{
					llPushBack($1, $3);
					$$ = $1;
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER_LIST, IDENTIFIER (%s)", astGetIdentifierName($3));
				}
               | IDENTIFIER
				{
					// Single item linked list
					$$ = llCreate();
					llPushBack($$, $1);
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER (%s)", astGetIdentifierName($1));
				}
               | /* empty */                      
				{
					$$ = llCreate(); // Empty list
					printRule(yylineno, "IDENTIFIER_LIST", "EMPTY");
				}
               ;
%%

void printRule(int lineno, const char* parent, const char* child, ...)
{
	va_list argptr;
    va_start(argptr, child);
	fprintf(stdout, "%-04d %-20s => ", lineno, parent);
    vfprintf(stdout, child, argptr);
    va_end(argptr);
	fprintf(stdout, "\n");
}

int yyerror(const char* msg)
{	
	fprintf(stderr, "SYNTAX ERROR (%d): %s\n", yylineno, msg);
	return 0;
}

int main(int argc,char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Missing input file.\n");
		return 1;
	}

	// Open input file
	if(!(yyin = fopen(argv[1], "r"))) {
		fprintf(stderr, "Cannot read file: %s\n", argv[1]);
		return 1;
	}

	// Redirect stdout to the second (optional) file.
	if(argc >= 3) {
		if(!freopen(argv[2], "w", stdout)) {
			fprintf(stderr, "Failed to redirect stdout to %s.\n", argv[2]);
			return 1;
		}
	}

	if(argc >= 4) {
		if(!freopen(argv[3], "w", stderr)) {
			fprintf(stderr, "Failed to redirect stderr to %s.\n", argv[3]);
			return 1;
		}
	}

	if(!yyparse()) {
		if(g_Program) {
			if(progBuildSymbolTable(g_Program)) {
				ICode* ic = icodeCompileProgram(g_Program);

				fprintf(stdout, "\n");
				fprintf(stdout, "Symbol Table\n");
				fprintf(stdout, "------------\n");
				progPrintSymbolTable(g_Program);
				fprintf(stdout, "\n");

				icodePrintQuads(ic);

				TCode* tc = tcodeCompileICode(ic, g_Program->m_SymbolTable);

				fprintf(stdout, "\n");
				tcodePrint(tc);

				char binaryFilename[256];
				sprintf(binaryFilename, "%s.abc", argv[1]);
				tcodeSaveBinary(tc, binaryFilename);

				tcodeDestroy(tc);
				icodeDestroy(ic);
			}
		}
	} else {
		fprintf(stderr, "Parsing failed.\n");
	}

	if(g_Program) {
		progDestroy(g_Program);
		g_Program = 0;
	}

	return 0;
}
