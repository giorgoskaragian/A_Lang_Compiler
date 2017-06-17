
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 8 "al.y"

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


/* Line 189 of yacc.c  */
#line 91 "parser.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 1 "al.y"

#include "ast.h"
#include "linked_list.h"
#include "icode.h"
#include "tcode.h"



/* Line 209 of yacc.c  */
#line 124 "parser.cpp"

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

/* Line 214 of yacc.c  */
#line 29 "al.y"

	char* strVal;
	int intVal;
	double doubleVal;
	struct ASTNode* astNode;
	struct LinkedList* list;
	struct Program* prog;



/* Line 214 of yacc.c  */
#line 201 "parser.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 213 "parser.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   620

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNRULES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    18,    26,    32,
      42,    46,    49,    52,    55,    59,    61,    63,    67,    71,
      75,    79,    83,    87,    91,    95,    99,   103,   107,   111,
     115,   119,   123,   126,   129,   132,   135,   138,   141,   143,
     145,   149,   153,   157,   159,   161,   163,   165,   167,   169,
     173,   175,   176,   182,   186,   188,   190,   193,   196,   200,
     205,   209,   214,   219,   224,   231,   238,   242,   249,   255,
     259,   261
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    52,    -1,    52,    53,    -1,    -1,    54,
      42,    -1,     7,    40,    54,    41,    53,    -1,     7,    40,
      54,    41,    53,     8,    53,    -1,     9,    40,    54,    41,
      53,    -1,    10,    40,    55,    42,    54,    42,    55,    41,
      53,    -1,    12,    54,    42,    -1,    12,    42,    -1,    13,
      42,    -1,    14,    42,    -1,    36,    52,    37,    -1,    61,
      -1,    42,    -1,    58,    22,    54,    -1,    54,    23,    54,
      -1,    54,    24,    54,    -1,    54,    25,    54,    -1,    54,
      26,    54,    -1,    54,    27,    54,    -1,    54,    32,    54,
      -1,    54,    34,    54,    -1,    54,    33,    54,    -1,    54,
      35,    54,    -1,    54,    28,    54,    -1,    54,    29,    54,
      -1,    54,    15,    54,    -1,    54,    17,    54,    -1,    40,
      54,    41,    -1,    24,    54,    -1,    16,    54,    -1,    30,
      58,    -1,    58,    30,    -1,    31,    58,    -1,    58,    31,
      -1,    58,    -1,    59,    -1,    38,    55,    39,    -1,    38,
      57,    39,    -1,    40,    61,    41,    -1,     3,    -1,     4,
      -1,     5,    -1,    21,    -1,    19,    -1,    20,    -1,    55,
      43,    54,    -1,    54,    -1,    -1,    36,    54,    44,    54,
      37,    -1,    57,    43,    56,    -1,    56,    -1,     6,    -1,
      18,     6,    -1,    45,     6,    -1,    58,    46,     6,    -1,
      58,    38,    54,    39,    -1,    59,    46,     6,    -1,    59,
      38,    54,    39,    -1,    59,    40,    55,    41,    -1,    58,
      40,    55,    41,    -1,    58,    47,     6,    40,    55,    41,
      -1,    40,    61,    41,    40,    55,    41,    -1,    36,    52,
      37,    -1,    11,     6,    40,    62,    41,    60,    -1,    11,
      40,    62,    41,    60,    -1,    62,    43,     6,    -1,     6,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    67,    67,    75,    83,    89,    94,    99,   104,   109,
     114,   119,   124,   129,   134,   139,   144,   151,   156,   161,
     166,   171,   176,   181,   186,   191,   196,   201,   206,   211,
     216,   221,   226,   231,   236,   241,   246,   251,   256,   261,
     266,   271,   276,   281,   286,   291,   296,   301,   306,   313,
     319,   326,   332,   339,   345,   353,   358,   363,   368,   373,
     378,   383,   390,   395,   400,   405,   412,   419,   424,   431,
     437,   445
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER_CONST", "REAL_CONST",
  "STRING_CONST", "IDENTIFIER", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR", "LOCAL", "TRUE",
  "FALSE", "NIL", "EQUAL", "PLUS", "MINUS", "MUL", "DIV", "MOD",
  "DOUBLE_EQUAL", "NOT_EQUAL", "INCREMENT", "DECREMENT", "GREATER_THAN",
  "LESS_THAN", "GREATER_THAN_OR_EQUAL", "LESS_THAN_OR_EQUAL",
  "OPEN_CURLY_BRACKET", "CLOSE_CURLY_BRACKET", "OPEN_BRACKET",
  "CLOSE_BRACKET", "OPEN_PARENTHESIS", "CLOSE_PARENTHESIS", "SEMICOLON",
  "COMMA", "COLON", "DOUBLE_COLON", "DOT", "DOUBLE_DOT", "UNARY_MINUS",
  "THEN", "$accept", "program", "statement_list", "statement",
  "expression", "expression_list", "indexed_element",
  "indexed_element_list", "lvalue", "call", "func_block",
  "function_definition", "identifier_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    55,
      55,    55,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    58,    59,    59,    59,    59,    60,    61,    61,    62,
      62,    62
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     5,     7,     5,     9,
       3,     2,     2,     2,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     3,
       1,     0,     5,     3,     1,     1,     2,     2,     3,     4,
       3,     4,     4,     4,     6,     6,     3,     6,     5,     3,
       1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,    43,    44,    45,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    46,
       0,     0,     0,     4,    51,     0,    16,     0,     3,     0,
      38,    39,    15,     0,     0,    51,     0,    71,    11,     0,
      12,    13,    33,    56,    32,     0,    34,     0,    36,     0,
       0,    50,     0,    54,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     0,    35,    37,     0,    51,     0,     0,     0,
      51,     0,     0,     0,     0,    71,    70,     0,    10,     0,
      14,     0,    40,     0,    41,     0,    31,    42,    29,    30,
      18,    19,    20,    21,    22,    27,    28,    23,    25,    24,
      26,    17,     0,     0,    58,     0,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    53,    51,
      59,    63,    51,    61,    62,     6,     8,     0,     0,     4,
      68,    69,     0,     0,     0,     0,    51,    67,     0,    52,
      65,    64,     7,     0,    66,     0,     9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    28,    29,    52,    53,    54,    30,    31,
     140,    32,    87
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -114
static const yytype_int16 yypact[] =
{
    -114,     4,   220,  -114,  -114,  -114,  -114,  -114,   -24,    -9,
      -3,    -5,    68,   -23,   -18,   309,    27,  -114,  -114,  -114,
     309,    -1,    -1,  -114,   263,   286,  -114,    34,  -114,   355,
      85,    37,  -114,   309,   309,   309,     5,    56,  -114,   376,
    -114,  -114,  -114,  -114,  -114,    54,   -20,    37,   -20,   134,
     309,   544,   -29,  -114,   -28,   418,    28,  -114,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   309,   309,
     309,  -114,   309,  -114,  -114,   309,   309,    76,   115,   309,
     309,   127,   439,   460,    48,    56,  -114,   -13,  -114,    35,
    -114,   333,  -114,   309,  -114,    49,  -114,    95,   572,   559,
      53,    53,  -114,  -114,  -114,   585,   585,    70,    70,    70,
      70,   544,   481,    -7,  -114,   102,   502,     0,  -114,   220,
     220,   309,    18,   113,   145,    95,   309,   544,  -114,   309,
    -114,  -114,   309,  -114,  -114,   148,  -114,   397,   113,  -114,
    -114,  -114,   523,    83,    86,   220,   309,  -114,   177,  -114,
    -114,  -114,  -114,    87,  -114,   220,  -114
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,   -21,  -113,   -12,   -10,    62,  -114,    79,    90,
      21,   -16,    75
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      39,    36,    49,    42,     3,     7,   135,   136,    44,    56,
      92,    94,    51,    55,    93,    95,    33,    16,    75,    40,
      76,    82,    83,    51,    41,    84,    77,    78,   123,    89,
     124,    34,   152,    43,   131,    37,    93,    35,    91,    45,
      57,   134,   156,    93,    27,    85,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   138,
     111,   124,    86,   112,    51,    11,   113,   116,    51,    97,
     117,     4,     5,     6,     7,    79,   125,    80,    62,    63,
      64,   127,   114,    81,    15,    50,    16,    17,    18,    19,
     121,    93,    20,    60,    61,    62,    63,    64,    21,    22,
      46,    48,    -1,    -1,    -1,    -1,    24,    72,    25,   137,
      38,    47,    47,    27,   142,    73,    74,    51,   148,   143,
      51,   115,   144,    75,   150,    76,    93,   151,   155,    93,
      93,    77,    78,   118,    51,   129,   153,     4,     5,     6,
       7,     8,   132,     9,    10,    11,    12,    13,    14,   139,
      15,   141,    16,    17,    18,    19,   145,   128,    20,   147,
     122,     0,     0,     0,    21,    22,     0,     0,     0,     0,
      23,    90,    24,     0,    25,     0,    26,     0,     0,    27,
       4,     5,     6,     7,     8,     0,     9,    10,    11,    12,
      13,    14,     0,    15,     0,    16,    17,    18,    19,     0,
       0,    20,     0,     0,     0,     0,     0,    21,    22,     0,
       0,     0,     0,    23,   154,    24,     0,    25,     0,    26,
       0,     0,    27,     4,     5,     6,     7,     8,     0,     9,
      10,    11,    12,    13,    14,     0,    15,     0,    16,    17,
      18,    19,     0,     0,    20,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,     0,    23,     0,    24,     0,
      25,     0,    26,     0,     0,    27,     4,     5,     6,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
       0,    16,    17,    18,    19,     0,     0,    20,     0,     4,
       5,     6,     7,    21,    22,     0,     0,    11,     0,    50,
       0,    24,    15,    25,    16,    17,    18,    19,    27,     0,
      20,     0,     4,     5,     6,     7,    21,    22,     0,     0,
       0,     0,     0,     0,    24,    15,    25,    16,    17,    18,
      19,    27,     0,    20,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    24,    58,    25,
      59,     0,     0,     0,    27,     0,    60,    61,    62,    63,
      64,    65,    66,     0,     0,    67,    68,    69,    70,     0,
      58,     0,    59,     0,     0,     0,     0,   126,    60,    61,
      62,    63,    64,    65,    66,     0,     0,    67,    68,    69,
      70,    58,     0,    59,     0,     0,     0,    71,     0,    60,
      61,    62,    63,    64,    65,    66,     0,     0,    67,    68,
      69,    70,    58,     0,    59,     0,     0,     0,    88,     0,
      60,    61,    62,    63,    64,    65,    66,     0,     0,    67,
      68,    69,    70,    58,     0,    59,     0,     0,     0,   146,
       0,    60,    61,    62,    63,    64,    65,    66,     0,     0,
      67,    68,    69,    70,    58,     0,    59,     0,     0,    96,
       0,     0,    60,    61,    62,    63,    64,    65,    66,     0,
       0,    67,    68,    69,    70,    58,     0,    59,     0,     0,
     119,     0,     0,    60,    61,    62,    63,    64,    65,    66,
       0,     0,    67,    68,    69,    70,    58,     0,    59,     0,
       0,   120,     0,     0,    60,    61,    62,    63,    64,    65,
      66,     0,     0,    67,    68,    69,    70,    58,     0,    59,
     130,     0,     0,     0,     0,    60,    61,    62,    63,    64,
      65,    66,     0,     0,    67,    68,    69,    70,    58,     0,
      59,   133,     0,     0,     0,     0,    60,    61,    62,    63,
      64,    65,    66,     0,     0,    67,    68,    69,    70,    58,
     149,    59,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,    65,    66,    58,     0,    67,    68,    69,    70,
       0,     0,    60,    61,    62,    63,    64,    65,    66,     0,
       0,    67,    68,    69,    70,    60,    61,    62,    63,    64,
      65,    66,     0,     0,    67,    68,    69,    70,    60,    61,
      62,    63,    64,    -1,    -1,     0,     0,    67,    68,    69,
      70
};

static const yytype_int16 yycheck[] =
{
      12,     6,    23,    15,     0,     6,   119,   120,    20,    25,
      39,    39,    24,    25,    43,    43,    40,    18,    38,    42,
      40,    33,    34,    35,    42,    35,    46,    47,    41,    45,
      43,    40,   145,     6,    41,    40,    43,    40,    50,    40,
       6,    41,   155,    43,    45,    40,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    41,
      72,    43,     6,    75,    76,    11,    76,    79,    80,    41,
      80,     3,     4,     5,     6,    38,    41,    40,    25,    26,
      27,    93,     6,    46,    16,    36,    18,    19,    20,    21,
      42,    43,    24,    23,    24,    25,    26,    27,    30,    31,
      21,    22,    32,    33,    34,    35,    38,    22,    40,   121,
      42,    21,    22,    45,   126,    30,    31,   129,   139,   129,
     132,     6,   132,    38,    41,    40,    43,    41,    41,    43,
      43,    46,    47,     6,   146,    40,   146,     3,     4,     5,
       6,     7,    40,     9,    10,    11,    12,    13,    14,    36,
      16,     6,    18,    19,    20,    21,     8,    95,    24,   138,
      85,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    -1,    42,    -1,    -1,    45,
       3,     4,     5,     6,     7,    -1,     9,    10,    11,    12,
      13,    14,    -1,    16,    -1,    18,    19,    20,    21,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    -1,    42,
      -1,    -1,    45,     3,     4,     5,     6,     7,    -1,     9,
      10,    11,    12,    13,    14,    -1,    16,    -1,    18,    19,
      20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    -1,
      40,    -1,    42,    -1,    -1,    45,     3,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    18,    19,    20,    21,    -1,    -1,    24,    -1,     3,
       4,     5,     6,    30,    31,    -1,    -1,    11,    -1,    36,
      -1,    38,    16,    40,    18,    19,    20,    21,    45,    -1,
      24,    -1,     3,     4,     5,     6,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    16,    40,    18,    19,    20,
      21,    45,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    15,    40,
      17,    -1,    -1,    -1,    45,    -1,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    33,    34,    35,    -1,
      15,    -1,    17,    -1,    -1,    -1,    -1,    44,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      33,    34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,
      -1,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    15,    -1,    17,    -1,    -1,    41,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    33,    34,    35,    15,    -1,    17,    -1,    -1,
      41,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    32,    33,    34,    35,    15,    -1,    17,    -1,
      -1,    41,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    15,    -1,    17,
      39,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    15,    -1,
      17,    39,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    33,    34,    35,    15,
      37,    17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    15,    -1,    32,    33,    34,    35,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    33,    34,    35,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    51,    52,     0,     3,     4,     5,     6,     7,     9,
      10,    11,    12,    13,    14,    16,    18,    19,    20,    21,
      24,    30,    31,    36,    38,    40,    42,    45,    53,    54,
      58,    59,    61,    40,    40,    40,     6,    40,    42,    54,
      42,    42,    54,     6,    54,    40,    58,    59,    58,    52,
      36,    54,    55,    56,    57,    54,    61,     6,    15,    17,
      23,    24,    25,    26,    27,    28,    29,    32,    33,    34,
      35,    42,    22,    30,    31,    38,    40,    46,    47,    38,
      40,    46,    54,    54,    55,    40,     6,    62,    42,    61,
      37,    54,    39,    43,    39,    43,    41,    41,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    55,     6,     6,    54,    55,     6,    41,
      41,    42,    62,    41,    43,    41,    44,    54,    56,    40,
      39,    41,    40,    39,    41,    53,    53,    54,    41,    36,
      60,     6,    54,    55,    55,     8,    42,    60,    52,    37,
      41,    41,    53,    55,    37,    41,    53
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

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

/* Line 1455 of yacc.c  */
#line 68 "al.y"
    { 
				(yyval.prog) = progCreate((yyvsp[(1) - (1)].list));
				g_Program = (yyval.prog);
				printRule(yylineno, "PROGRAM", "STATEMENT_LIST"); 
			;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 76 "al.y"
    { 
					llPushBack((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].astNode));
					(yyval.list) = (yyvsp[(1) - (2)].list);

					printRule(yylineno, "STATEMENT_LIST", "STATEMENT_LIST STATEMENT"); 
				;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 83 "al.y"
    { 
					(yyval.list) = llCreate();
					printRule(yylineno, "STATEMENT_LIST", "EMPTY");                    
				;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 90 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Expression(yylineno, (yyvsp[(1) - (2)].astNode));
				printRule(yylineno, "STATEMENT", "EXPRESSION;"); 
			;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 95 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_If(yylineno, (yyvsp[(3) - (5)].astNode), (yyvsp[(5) - (5)].astNode), 0);
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT"); 
			;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 100 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_If(yylineno, (yyvsp[(3) - (7)].astNode), (yyvsp[(5) - (7)].astNode), (yyvsp[(7) - (7)].astNode));
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT else STATEMENT"); 
			;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 105 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_While(yylineno, (yyvsp[(3) - (5)].astNode), (yyvsp[(5) - (5)].astNode));
				printRule(yylineno, "STATEMENT", "while (EXPRESSION) STATEMENT"); 
			;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 110 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_For(yylineno, (yyvsp[(3) - (9)].list), (yyvsp[(5) - (9)].astNode), (yyvsp[(7) - (9)].list), (yyvsp[(9) - (9)].astNode));
				printRule(yylineno, "STATEMENT", "for (EXPRESSION_LIST; EXPRESSION; EXPRESSION_LIST) STATEMENT"); 
			;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 115 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Return(yylineno, (yyvsp[(2) - (3)].astNode));
				printRule(yylineno, "STATEMENT", "return EXPRESSION;"); 
			;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 120 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Return(yylineno, 0);
				printRule(yylineno, "STATEMENT", "return;"); 
			;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 125 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Break);
				printRule(yylineno, "STATEMENT", "break;"); 
			;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 130 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Continue);
				printRule(yylineno, "STATEMENT", "continue;"); 
			;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 135 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Block(yylineno, (yyvsp[(2) - (3)].list));
				printRule(yylineno, "STATEMENT", "{ STATEMENT_LIST }"); 
			;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 140 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_FuncDef(yylineno, (yyvsp[(1) - (1)].astNode));
				printRule(yylineno, "STATEMENT", "FUNC_DEF"); 
			;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 145 "al.y"
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Empty);
				printRule(yylineno, "STATEMENT", ";"); 
			;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 152 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Assignment(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode));
				printRule(yylineno, "EXPRESSION", "LVALUE = EXPRESSION");       
			;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 157 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), Op_Add);
				printRule(yylineno, "EXPRESSION", "EXPRESSION + EXPRESSION");   
			;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 162 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), Op_Subtract);
				printRule(yylineno, "EXPRESSION", "EXPRESSION - EXPRESSION");   
			;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 167 "al.y"
    {
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), Op_Multiply);
				printRule(yylineno, "EXPRESSION", "EXPRESSION * EXPRESSION");   
			;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 172 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), Op_Divide);
				printRule(yylineno, "EXPRESSION", "EXPRESSION / EXPRESSION");   
			;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 177 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), Op_Modulo);
				printRule(yylineno, "EXPRESSION", "EXPRESSION % EXPRESSION");   
			;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 182 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_GreaterThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION > EXPRESSION");   
			;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 187 "al.y"
    {
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_GreaterThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION >= EXPRESSION");
			;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 192 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_LessThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION < EXPRESSION");   
			;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 197 "al.y"
    {
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_LessThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION <= EXPRESSION");
			;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 202 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_Equal);
				printRule(yylineno, "EXPRESSION", "EXPRESSION == EXPRESSION");  
			;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 207 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_NotEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION != EXPRESSION");  
			;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 212 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_And);
				printRule(yylineno, "EXPRESSION", "EXPRESSION and EXPRESSION"); 
			;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 217 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode), LogicOp_Or);
				printRule(yylineno, "EXPRESSION", "EXPRESSION or EXPRESSION");  
			;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 222 "al.y"
    {
				(yyval.astNode) = astCreateExpression_Nested(yylineno, (yyvsp[(2) - (3)].astNode));
				printRule(yylineno, "EXPRESSION", "(EXPRESSION)");
			;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 227 "al.y"
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(2) - (2)].astNode), UnaryOp_Minus);
				printRule(yylineno, "EXPRESSION", "-EXPRESSION");
			;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 232 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(2) - (2)].astNode), UnaryOp_Not);
				printRule(yylineno, "EXPRESSION", "not EXPRESSION");
			;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 237 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(2) - (2)].astNode), UnaryOp_PreInc);
				printRule(yylineno, "EXPRESSION", "++LVALUE");
			;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 242 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(1) - (2)].astNode), UnaryOp_PostInc);
				printRule(yylineno, "EXPRESSION", "LVALUE++");
			;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 247 "al.y"
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(2) - (2)].astNode), UnaryOp_PreDec);
				printRule(yylineno, "EXPRESSION", "--LVALUE");
			;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 252 "al.y"
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[(1) - (2)].astNode), UnaryOp_PostDec);
				printRule(yylineno, "EXPRESSION", "LVALUE--");
			;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 257 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_lValue(yylineno, (yyvsp[(1) - (1)].astNode));
				printRule(yylineno, "EXPRESSION", "LVALUE");
			;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 262 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_Call(yylineno, (yyvsp[(1) - (1)].astNode));
				printRule(yylineno, "EXPRESSION", "CALL");
			;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 267 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_ObjDef(yylineno, (yyvsp[(2) - (3)].list), ObjDef_ExpressionList);
				printRule(yylineno, "EXPRESSION", "[EXPRESSION_LIST]");
			;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 272 "al.y"
    { 
				(yyval.astNode) = astCreateExpression_ObjDef(yylineno, (yyvsp[(2) - (3)].list), ObjDef_IndexedElementList);
				printRule(yylineno, "EXPRESSION", "[INDEXED_ELEMENT_LIST]");
			;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 277 "al.y"
    {
				(yyval.astNode) = astCreateExpression_FuncDef(yylineno, (yyvsp[(2) - (3)].astNode));
				printRule(yylineno, "EXPRESSION", "(FUNC_DEF)");
			;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 282 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstInt(yylineno, (yyvsp[(1) - (1)].intVal));
				printRule(yylineno, "EXPRESSION", "CONST INTEGER (%d)", (yyvsp[(1) - (1)].intVal));
			;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 287 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstReal(yylineno, (yyvsp[(1) - (1)].doubleVal));
				printRule(yylineno, "EXPRESSION", "CONST REAL (%f)", (yyvsp[(1) - (1)].doubleVal));
			;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 292 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstString(yylineno, (yyvsp[(1) - (1)].strVal));
				printRule(yylineno, "EXPRESSION", "CONST STRING (\"%s\")", (yyvsp[(1) - (1)].strVal));
			;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 297 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstNil(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST nil");
			;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 302 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstTrue(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST true");
			;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 307 "al.y"
    {
				(yyval.astNode) = astCreateExpression_ConstFalse(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST false");
			;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 314 "al.y"
    {
					llPushBack((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].astNode));
					(yyval.list) = (yyvsp[(1) - (3)].list);
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION_LIST, EXPRESSION");
				;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 320 "al.y"
    { 
					(yyval.list) = llCreate();
					llPushBack((yyval.list), (yyvsp[(1) - (1)].astNode));
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION");
				;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 326 "al.y"
    { 
					(yyval.list) = llCreate();
					printRule(yylineno, "EXPRESSION_LIST", "EMPTY");
				;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 333 "al.y"
    {
					(yyval.astNode) = astCreateIndexedElement(yylineno, (yyvsp[(2) - (5)].astNode), (yyvsp[(4) - (5)].astNode));
					printRule(yylineno, "INDEXED_ELEMENT", "{ EXPRESSION:EXPRESSION }");
				;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 340 "al.y"
    {
						llPushBack((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].astNode));
						(yyval.list) = (yyvsp[(1) - (3)].list);
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT_LIST, INDEXED_ELEMENT");
					;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 346 "al.y"
    {
						(yyval.list) = llCreate();
						llPushBack((yyval.list), (yyvsp[(1) - (1)].astNode));
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT");
					;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 354 "al.y"
    {
			(yyval.astNode) = astCreateLValue_Identifier(yylineno, (yyvsp[(1) - (1)].astNode));
			printRule(yylineno, "LVALUE", "IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(1) - (1)].astNode)));
		;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 359 "al.y"
    {
			(yyval.astNode) = astCreateLValue_LocalIdentifier(yylineno, (yyvsp[(2) - (2)].astNode));
			printRule(yylineno, "LVALUE", "local IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(2) - (2)].astNode)));
		;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 364 "al.y"
    {
			(yyval.astNode) = astCreateLValue_GlobalIdentifier(yylineno, (yyvsp[(2) - (2)].astNode));
			printRule(yylineno, "LVALUE", "::IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(2) - (2)].astNode)));
		;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 369 "al.y"
    {
			(yyval.astNode) = astCreateLValue_ObjectMember(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode));
			printRule(yylineno, "LVALUE", "LVALUE.IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(3) - (3)].astNode)));
		;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 374 "al.y"
    {
			(yyval.astNode) = astCreateLValue_ArrayElement(yylineno, (yyvsp[(1) - (4)].astNode), (yyvsp[(3) - (4)].astNode));
			printRule(yylineno, "LVALUE", "LVALUE[EXPRESSION]");
		;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 379 "al.y"
    {
			(yyval.astNode) = astCreateLValue_CallObjectMember(yylineno, (yyvsp[(1) - (3)].astNode), (yyvsp[(3) - (3)].astNode));
			printRule(yylineno, "LVALUE", "CALL.IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(3) - (3)].astNode)));
		;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 384 "al.y"
    {
			(yyval.astNode) = astCreateLValue_CallArrayElement(yylineno, (yyvsp[(1) - (4)].astNode), (yyvsp[(3) - (4)].astNode));
			printRule(yylineno, "LVALUE", "CALL[EXPRESSION]");
		;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 391 "al.y"
    {
			(yyval.astNode) = astCreateCall_Call(yylineno, (yyvsp[(1) - (4)].astNode), (yyvsp[(3) - (4)].list));
			printRule(yylineno, "CALL", "CALL(EXPRESSION_LIST)");
		;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 396 "al.y"
    {
			(yyval.astNode) = astCreateCall_Normal(yylineno, (yyvsp[(1) - (4)].astNode), (yyvsp[(3) - (4)].list));
			printRule(yylineno, "CALL", "LVALUE(EXPRESSION_LIST)");
		;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 401 "al.y"
    {
			(yyval.astNode) = astCreateCall_Method(yylineno, (yyvsp[(1) - (6)].astNode), (yyvsp[(3) - (6)].astNode), (yyvsp[(5) - (6)].list));
			printRule(yylineno, "CALL", "LVALUE..IDENTIFIER(EXPRESSION_LIST) (%s)", astGetIdentifierName((yyvsp[(3) - (6)].astNode)));
		;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 406 "al.y"
    {
			(yyval.astNode) = astCreateCall_Lambda(yylineno, (yyvsp[(2) - (6)].astNode), (yyvsp[(5) - (6)].list));
			printRule(yylineno, "CALL", "(FUNC_DEF)(EXPRESSION_LIST)");
		;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 413 "al.y"
    { 
			(yyval.list) = (yyvsp[(2) - (3)].list);
			printRule(yylineno, "FUNC_BLOCK", "{ STATEMENT_LIST }");
		;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 420 "al.y"
    { 
						(yyval.astNode) = astCreateFuncDef(yylineno, (yyvsp[(2) - (6)].astNode), (yyvsp[(4) - (6)].list), (yyvsp[(6) - (6)].list));
						printRule(yylineno, "FUNC_DEF", "function IDENTIFIER (%s) (IDENTIFIER_LIST) FUNC_BLOCK", astGetIdentifierName((yyvsp[(2) - (6)].astNode)));
					;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 425 "al.y"
    {
						(yyval.astNode) = astCreateFuncDef(yylineno, 0, (yyvsp[(3) - (5)].list), (yyvsp[(5) - (5)].list));
						printRule(yylineno, "FUNC_DEF", "function (IDENTIFIER_LIST) FUNC_BLOCK");
					;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 432 "al.y"
    {
					llPushBack((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].astNode));
					(yyval.list) = (yyvsp[(1) - (3)].list);
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER_LIST, IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(3) - (3)].astNode)));
				;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 438 "al.y"
    {
					// Single item linked list
					(yyval.list) = llCreate();
					llPushBack((yyval.list), (yyvsp[(1) - (1)].astNode));
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER (%s)", astGetIdentifierName((yyvsp[(1) - (1)].astNode)));
				;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 445 "al.y"
    {
					(yyval.list) = llCreate(); // Empty list
					printRule(yylineno, "IDENTIFIER_LIST", "EMPTY");
				;}
    break;



/* Line 1455 of yacc.c  */
#line 2345 "parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 450 "al.y"


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

