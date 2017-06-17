
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 1 "al.y"

#include "ast.h"
#include "linked_list.h"
#include "icode.h"
#include "tcode.h"



/* Line 1676 of yacc.c  */
#line 49 "parser.hpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER_CONST = 258,
     REAL_CONST = 259,
     STRING_CONST = 260,
     IDENTIFIER = 261,
     IF = 262,
     ELSE = 263,
     WHILE = 264,
     FOR = 265,
     FUNCTION = 266,
     RETURN = 267,
     BREAK = 268,
     CONTINUE = 269,
     AND = 270,
     NOT = 271,
     OR = 272,
     LOCAL = 273,
     TRUE = 274,
     FALSE = 275,
     NIL = 276,
     EQUAL = 277,
     PLUS = 278,
     MINUS = 279,
     MUL = 280,
     DIV = 281,
     MOD = 282,
     DOUBLE_EQUAL = 283,
     NOT_EQUAL = 284,
     INCREMENT = 285,
     DECREMENT = 286,
     GREATER_THAN = 287,
     LESS_THAN = 288,
     GREATER_THAN_OR_EQUAL = 289,
     LESS_THAN_OR_EQUAL = 290,
     OPEN_CURLY_BRACKET = 291,
     CLOSE_CURLY_BRACKET = 292,
     OPEN_BRACKET = 293,
     CLOSE_BRACKET = 294,
     OPEN_PARENTHESIS = 295,
     CLOSE_PARENTHESIS = 296,
     SEMICOLON = 297,
     COMMA = 298,
     COLON = 299,
     DOUBLE_COLON = 300,
     DOT = 301,
     DOUBLE_DOT = 302,
     UNARY_MINUS = 303,
     THEN = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 29 "al.y"

	char* strVal;
	int intVal;
	double doubleVal;
	struct ASTNode* astNode;
	struct LinkedList* list;
	struct Program* prog;



/* Line 1676 of yacc.c  */
#line 126 "parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


