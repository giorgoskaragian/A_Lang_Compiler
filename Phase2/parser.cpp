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
#line 6 "al.y" /* yacc.c:339  */

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

#line 83 "parser.cpp" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "al.y" /* yacc.c:355  */

#include "ast.h"
#include "linked_list.h"

#line 118 "parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 27 "al.y" /* yacc.c:355  */

	char* strVal;
	int intVal;
	double doubleVal;
	struct ASTNode* astNode;
	struct LinkedList* list;
	struct Program* prog;

#line 189 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 206 "parser.cpp" /* yacc.c:358  */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   620

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    65,    65,    73,    81,    87,    92,    97,   102,   107,
     112,   117,   122,   127,   132,   137,   142,   149,   154,   159,
     164,   169,   174,   179,   184,   189,   194,   199,   204,   209,
     214,   219,   224,   229,   234,   239,   244,   249,   254,   259,
     264,   269,   274,   279,   284,   289,   294,   299,   304,   311,
     317,   324,   330,   337,   343,   351,   356,   361,   366,   371,
     376,   381,   388,   393,   398,   403,   410,   417,   422,   429,
     435,   443
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
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
  "function_definition", "identifier_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

#define YYPACT_NINF -114

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-114)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
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

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
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

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,   -21,  -113,   -12,   -10,    62,  -114,    79,    90,
      21,   -16,    75
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    28,    29,    52,    53,    54,    30,    31,
     140,    32,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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
#line 66 "al.y" /* yacc.c:1646  */
    { 
				(yyval.prog) = progCreate((yyvsp[0].list));
				g_Program = (yyval.prog);
				printRule(yylineno, "PROGRAM", "STATEMENT_LIST"); 
			}
#line 1487 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 74 "al.y" /* yacc.c:1646  */
    { 
					llPushBack((yyvsp[-1].list), (yyvsp[0].astNode));
					(yyval.list) = (yyvsp[-1].list);

					printRule(yylineno, "STATEMENT_LIST", "STATEMENT_LIST STATEMENT"); 
				}
#line 1498 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 81 "al.y" /* yacc.c:1646  */
    { 
					(yyval.list) = llCreate();
					printRule(yylineno, "STATEMENT_LIST", "EMPTY");                    
				}
#line 1507 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 88 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Expression(yylineno, (yyvsp[-1].astNode));
				printRule(yylineno, "STATEMENT", "EXPRESSION;"); 
			}
#line 1516 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 93 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_If(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), 0);
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT"); 
			}
#line 1525 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 98 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_If(yylineno, (yyvsp[-4].astNode), (yyvsp[-2].astNode), (yyvsp[0].astNode));
				printRule(yylineno, "STATEMENT", "if (EXPRESSION) STATEMENT else STATEMENT"); 
			}
#line 1534 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 103 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_While(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode));
				printRule(yylineno, "STATEMENT", "while (EXPRESSION) STATEMENT"); 
			}
#line 1543 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 108 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_For(yylineno, (yyvsp[-6].list), (yyvsp[-4].astNode), (yyvsp[-2].list), (yyvsp[0].astNode));
				printRule(yylineno, "STATEMENT", "for (EXPRESSION_LIST; EXPRESSION; EXPRESSION_LIST) STATEMENT"); 
			}
#line 1552 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 113 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Return(yylineno, (yyvsp[-1].astNode));
				printRule(yylineno, "STATEMENT", "return EXPRESSION;"); 
			}
#line 1561 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 118 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Return(yylineno, 0);
				printRule(yylineno, "STATEMENT", "return;"); 
			}
#line 1570 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 123 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Break);
				printRule(yylineno, "STATEMENT", "break;"); 
			}
#line 1579 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Continue);
				printRule(yylineno, "STATEMENT", "continue;"); 
			}
#line 1588 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 133 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Block(yylineno, (yyvsp[-1].list));
				printRule(yylineno, "STATEMENT", "{ STATEMENT_LIST }"); 
			}
#line 1597 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 138 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_FuncDef(yylineno, (yyvsp[0].astNode));
				printRule(yylineno, "STATEMENT", "FUNC_DEF"); 
			}
#line 1606 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 143 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateStatement_Generic(yylineno, ST_Empty);
				printRule(yylineno, "STATEMENT", ";"); 
			}
#line 1615 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 150 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Assignment(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode));
				printRule(yylineno, "EXPRESSION", "LVALUE = EXPRESSION");       
			}
#line 1624 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 155 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), Op_Add);
				printRule(yylineno, "EXPRESSION", "EXPRESSION + EXPRESSION");   
			}
#line 1633 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 160 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), Op_Subtract);
				printRule(yylineno, "EXPRESSION", "EXPRESSION - EXPRESSION");   
			}
#line 1642 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 165 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), Op_Multiply);
				printRule(yylineno, "EXPRESSION", "EXPRESSION * EXPRESSION");   
			}
#line 1651 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 170 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), Op_Divide);
				printRule(yylineno, "EXPRESSION", "EXPRESSION / EXPRESSION");   
			}
#line 1660 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 175 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Op(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), Op_Modulo);
				printRule(yylineno, "EXPRESSION", "EXPRESSION % EXPRESSION");   
			}
#line 1669 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 180 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_GreaterThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION > EXPRESSION");   
			}
#line 1678 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 185 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_GreaterThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION >= EXPRESSION");
			}
#line 1687 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 190 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_LessThan);
				printRule(yylineno, "EXPRESSION", "EXPRESSION < EXPRESSION");   
			}
#line 1696 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 195 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_LessThanOrEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION <= EXPRESSION");
			}
#line 1705 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 200 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_Equal);
				printRule(yylineno, "EXPRESSION", "EXPRESSION == EXPRESSION");  
			}
#line 1714 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 205 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_NotEqual);
				printRule(yylineno, "EXPRESSION", "EXPRESSION != EXPRESSION");  
			}
#line 1723 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 210 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_And);
				printRule(yylineno, "EXPRESSION", "EXPRESSION and EXPRESSION"); 
			}
#line 1732 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 215 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_LogicOp(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode), LogicOp_Or);
				printRule(yylineno, "EXPRESSION", "EXPRESSION or EXPRESSION");  
			}
#line 1741 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 220 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_Nested(yylineno, (yyvsp[-1].astNode));
				printRule(yylineno, "EXPRESSION", "(EXPRESSION)");
			}
#line 1750 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 225 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[0].astNode), UnaryOp_Minus);
				printRule(yylineno, "EXPRESSION", "-EXPRESSION");
			}
#line 1759 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 230 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[0].astNode), UnaryOp_Not);
				printRule(yylineno, "EXPRESSION", "not EXPRESSION");
			}
#line 1768 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 235 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[0].astNode), UnaryOp_PreInc);
				printRule(yylineno, "EXPRESSION", "++LVALUE");
			}
#line 1777 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 240 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[-1].astNode), UnaryOp_PostInc);
				printRule(yylineno, "EXPRESSION", "LVALUE++");
			}
#line 1786 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 245 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[0].astNode), UnaryOp_PreDec);
				printRule(yylineno, "EXPRESSION", "--LVALUE");
			}
#line 1795 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 250 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_UnaryOp(yylineno, (yyvsp[-1].astNode), UnaryOp_PostDec);
				printRule(yylineno, "EXPRESSION", "LVALUE--");
			}
#line 1804 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 255 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_lValue(yylineno, (yyvsp[0].astNode));
				printRule(yylineno, "EXPRESSION", "LVALUE");
			}
#line 1813 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 260 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_Call(yylineno, (yyvsp[0].astNode));
				printRule(yylineno, "EXPRESSION", "CALL");
			}
#line 1822 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 265 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_ObjDef(yylineno, (yyvsp[-1].list), ObjDef_ExpressionList);
				printRule(yylineno, "EXPRESSION", "[EXPRESSION_LIST]");
			}
#line 1831 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 270 "al.y" /* yacc.c:1646  */
    { 
				(yyval.astNode) = astCreateExpression_ObjDef(yylineno, (yyvsp[-1].list), ObjDef_IndexedElementList);
				printRule(yylineno, "EXPRESSION", "[INDEXED_ELEMENT_LIST]");
			}
#line 1840 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 275 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_FuncDef(yylineno, (yyvsp[-1].astNode));
				printRule(yylineno, "EXPRESSION", "(FUNC_DEF)");
			}
#line 1849 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 280 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstInt(yylineno, (yyvsp[0].intVal));
				printRule(yylineno, "EXPRESSION", "CONST INTEGER (%d)", (yyvsp[0].intVal));
			}
#line 1858 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 285 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstReal(yylineno, (yyvsp[0].doubleVal));
				printRule(yylineno, "EXPRESSION", "CONST REAL (%f)", (yyvsp[0].doubleVal));
			}
#line 1867 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 290 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstString(yylineno, (yyvsp[0].strVal));
				printRule(yylineno, "EXPRESSION", "CONST STRING (\"%s\")", (yyvsp[0].strVal));
			}
#line 1876 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 295 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstNil(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST nil");
			}
#line 1885 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 300 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstTrue(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST true");
			}
#line 1894 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 305 "al.y" /* yacc.c:1646  */
    {
				(yyval.astNode) = astCreateExpression_ConstFalse(yylineno);
				printRule(yylineno, "EXPRESSION", "CONST false");
			}
#line 1903 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 312 "al.y" /* yacc.c:1646  */
    {
					llPushBack((yyvsp[-2].list), (yyvsp[0].astNode));
					(yyval.list) = (yyvsp[-2].list);
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION_LIST, EXPRESSION");
				}
#line 1913 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 318 "al.y" /* yacc.c:1646  */
    { 
					(yyval.list) = llCreate();
					llPushBack((yyval.list), (yyvsp[0].astNode));
					printRule(yylineno, "EXPRESSION_LIST", "EXPRESSION");
				}
#line 1923 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 324 "al.y" /* yacc.c:1646  */
    { 
					(yyval.list) = llCreate();
					printRule(yylineno, "EXPRESSION_LIST", "EMPTY");
				}
#line 1932 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 331 "al.y" /* yacc.c:1646  */
    {
					(yyval.astNode) = astCreateIndexedElement(yylineno, (yyvsp[-3].astNode), (yyvsp[-1].astNode));
					printRule(yylineno, "INDEXED_ELEMENT", "{ EXPRESSION:EXPRESSION }");
				}
#line 1941 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 338 "al.y" /* yacc.c:1646  */
    {
						llPushBack((yyvsp[-2].list), (yyvsp[0].astNode));
						(yyval.list) = (yyvsp[-2].list);
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT_LIST, INDEXED_ELEMENT");
					}
#line 1951 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 344 "al.y" /* yacc.c:1646  */
    {
						(yyval.list) = llCreate();
						llPushBack((yyval.list), (yyvsp[0].astNode));
						printRule(yylineno, "INDEXED_ELEMENT_LIST", "INDEXED_ELEMENT");
					}
#line 1961 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 352 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_Identifier(yylineno, (yyvsp[0].astNode));
			printRule(yylineno, "LVALUE", "IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
		}
#line 1970 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 357 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_LocalIdentifier(yylineno, (yyvsp[0].astNode));
			printRule(yylineno, "LVALUE", "local IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
		}
#line 1979 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 362 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_GlobalIdentifier(yylineno, (yyvsp[0].astNode));
			printRule(yylineno, "LVALUE", "::IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
		}
#line 1988 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 367 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_ObjectMember(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode));
			printRule(yylineno, "LVALUE", "LVALUE.IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
		}
#line 1997 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 372 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_ArrayElement(yylineno, (yyvsp[-3].astNode), (yyvsp[-1].astNode));
			printRule(yylineno, "LVALUE", "LVALUE[EXPRESSION]");
		}
#line 2006 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 377 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_CallObjectMember(yylineno, (yyvsp[-2].astNode), (yyvsp[0].astNode));
			printRule(yylineno, "LVALUE", "CALL.IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
		}
#line 2015 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 382 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateLValue_CallArrayElement(yylineno, (yyvsp[-3].astNode), (yyvsp[-1].astNode));
			printRule(yylineno, "LVALUE", "CALL[EXPRESSION]");
		}
#line 2024 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 389 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateCall_Call(yylineno, (yyvsp[-3].astNode), (yyvsp[-1].list));
			printRule(yylineno, "CALL", "CALL(EXPRESSION_LIST)");
		}
#line 2033 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 394 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateCall_Normal(yylineno, (yyvsp[-3].astNode), (yyvsp[-1].list));
			printRule(yylineno, "CALL", "LVALUE(EXPRESSION_LIST)");
		}
#line 2042 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 399 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateCall_Method(yylineno, (yyvsp[-5].astNode), (yyvsp[-3].astNode), (yyvsp[-1].list));
			printRule(yylineno, "CALL", "LVALUE..IDENTIFIER(EXPRESSION_LIST) (%s)", astGetIdentifierName((yyvsp[-3].astNode)));
		}
#line 2051 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 404 "al.y" /* yacc.c:1646  */
    {
			(yyval.astNode) = astCreateCall_Lambda(yylineno, (yyvsp[-4].astNode), (yyvsp[-1].list));
			printRule(yylineno, "CALL", "(FUNC_DEF)(EXPRESSION_LIST)");
		}
#line 2060 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 411 "al.y" /* yacc.c:1646  */
    { 
			(yyval.list) = (yyvsp[-1].list);
			printRule(yylineno, "FUNC_BLOCK", "{ STATEMENT_LIST }");
		}
#line 2069 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 418 "al.y" /* yacc.c:1646  */
    { 
						(yyval.astNode) = astCreateFuncDef(yylineno, (yyvsp[-4].astNode), (yyvsp[-2].list), (yyvsp[0].list));
						printRule(yylineno, "FUNC_DEF", "function IDENTIFIER (%s) (IDENTIFIER_LIST) FUNC_BLOCK", astGetIdentifierName((yyvsp[-4].astNode)));
					}
#line 2078 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 423 "al.y" /* yacc.c:1646  */
    {
						(yyval.astNode) = astCreateFuncDef(yylineno, 0, (yyvsp[-2].list), (yyvsp[0].list));
						printRule(yylineno, "FUNC_DEF", "function (IDENTIFIER_LIST) FUNC_BLOCK");
					}
#line 2087 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 430 "al.y" /* yacc.c:1646  */
    {
					llPushBack((yyvsp[-2].list), (yyvsp[0].astNode));
					(yyval.list) = (yyvsp[-2].list);
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER_LIST, IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
				}
#line 2097 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 436 "al.y" /* yacc.c:1646  */
    {
					// Single item linked list
					(yyval.list) = llCreate();
					llPushBack((yyval.list), (yyvsp[0].astNode));
					printRule(yylineno, "IDENTIFIER_LIST", "IDENTIFIER (%s)", astGetIdentifierName((yyvsp[0].astNode)));
				}
#line 2108 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 443 "al.y" /* yacc.c:1646  */
    {
					(yyval.list) = llCreate(); // Empty list
					printRule(yylineno, "IDENTIFIER_LIST", "EMPTY");
				}
#line 2117 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2121 "parser.cpp" /* yacc.c:1646  */
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
#line 448 "al.y" /* yacc.c:1906  */


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
			progBuildSymbolTable(g_Program);

			fprintf(stdout, "\n");
			fprintf(stdout, "Symbol Table\n");
			fprintf(stdout, "------------\n");
			progPrintSymbolTable(g_Program);
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
