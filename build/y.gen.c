
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
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         konoha_parse
#define yylex           konoha_lex
#define yyerror         konoha_error
#define yylval          konoha_lval
#define yychar          konoha_char
#define yydebug         konoha_debug
#define yynerrs         konoha_nerrs
#define yylloc          konoha_lloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "konoha-paser.y"


#include "konoha.h"

#ifndef yyparse
extern int yyparse(void*);
extern int yylex(void*);
#endif
#define YYPARSE_PARAM ctx
#define YYLEX_PARAM   ctx

#define TODO() {\
    konoha_error("TODO");\
    asm volatile("int3");\
}
#define debug0 debug
#define debug(fmt, ...)   dbg_p(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

DEF_ARRAY_STRUCT(Token);
DEF_ARRAY_OP(Token);
DEF_TUPLE_STRUCT(Token, Token);
DEF_TUPLE_OP(Token, Token);



/* Line 189 of yacc.c  */
#line 107 "build/y.gen.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IF_WITHOUT_ELSE = 258,
     ELSE = 259,
     CAMMA = 260,
     COLON = 261,
     SEMICOLON = 262,
     LBRACE = 263,
     RBRACE = 264,
     LCBRACE = 265,
     RCBRACE = 266,
     LPARENTHESIS = 267,
     RPARENTHESIS = 268,
     Var = 269,
     If = 270,
     Else = 271,
     Do = 272,
     While = 273,
     For = 274,
     ForEach = 275,
     In = 276,
     Switch = 277,
     Case = 278,
     Break = 279,
     Default = 280,
     Continue = 281,
     Function = 282,
     Return = 283,
     This = 284,
     Try = 285,
     CATCH = 286,
     FINALLY = 287,
     Throw = 288,
     Typeof = 289,
     Instanceof = 290,
     New = 291,
     T_NULL = 292,
     T_TRUE = 293,
     T_FALSE = 294,
     IdentifierName = 295,
     HexIntegerLiteral = 296,
     IntegerLiteral = 297,
     FloatLiteral = 298,
     StringLiteral = 299,
     PLUSPLUS = 300,
     MINUSMINUS = 301,
     EQ_LET = 302,
     MUL_LET = 303,
     DIV_LET = 304,
     REM_LET = 305,
     ADD_LET = 306,
     SUB_LET = 307,
     LSHIFT_LET = 308,
     RSHIFT_LET = 309,
     AND_LET = 310,
     OR_LET = 311,
     XOR_LET = 312,
     DOT = 313,
     QUESTION = 314,
     AND = 315,
     LAND = 316,
     OR = 317,
     LOR = 318,
     XOR = 319,
     ADD = 320,
     SUB = 321,
     MUL = 322,
     DIV = 323,
     REM = 324,
     INV = 325,
     NOT = 326,
     LT = 327,
     LTE = 328,
     GT = 329,
     GTE = 330,
     LSHIFT = 331,
     RSHIFT = 332,
     EQEQ = 333,
     NEQ = 334
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 27 "konoha-paser.y"

  Array(Token) *vec;
  knh_Token_t *token;
  KOperator op;



/* Line 214 of yacc.c  */
#line 230 "build/y.gen.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 255 "build/y.gen.c"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  133
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1044

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  193
/* YYNRULES -- Number of states.  */
#define YYNSTATES  372

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   336

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    24,    26,    28,    31,    33,    35,    37,
      39,    41,    43,    45,    47,    49,    51,    53,    55,    57,
      59,    61,    63,    67,    68,    70,    73,    77,    79,    82,
      86,    90,    92,    96,    98,   101,   104,   106,   110,   112,
     114,   116,   118,   120,   122,   124,   126,   128,   130,   132,
     134,   140,   142,   145,   151,   159,   167,   173,   183,   194,
     202,   211,   214,   218,   221,   225,   228,   232,   238,   242,
     248,   249,   251,   255,   260,   263,   267,   271,   274,   278,
     282,   287,   293,   296,   300,   302,   304,   309,   318,   327,
     335,   344,   345,   347,   349,   353,   354,   356,   358,   360,
     362,   364,   366,   370,   373,   377,   381,   387,   390,   395,
     396,   398,   400,   403,   406,   410,   415,   417,   421,   425,
     433,   442,   444,   446,   448,   450,   452,   454,   456,   461,
     465,   469,   471,   474,   477,   480,   485,   489,   492,   496,
     498,   502,   504,   506,   508,   511,   514,   516,   519,   522,
     525,   528,   531,   534,   537,   539,   543,   547,   551,   553,
     557,   561,   563,   567,   571,   573,   577,   581,   585,   589,
     593,   597,   599,   603,   607,   609,   613,   615,   619,   621,
     625,   627,   631,   633
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      87,     0,    -1,    84,    -1,    85,    -1,    86,    -1,    44,
      -1,    37,    -1,    38,    -1,    39,    -1,    42,    -1,    43,
      -1,    41,    -1,    -1,    88,    -1,    89,    -1,    88,    89,
      -1,    90,    -1,   121,    -1,    93,    -1,    95,    -1,   102,
      -1,   103,    -1,   104,    -1,   105,    -1,   106,    -1,   107,
      -1,   108,    -1,   114,    -1,   109,    -1,   115,    -1,   116,
      -1,    99,    -1,    91,     5,    99,    -1,    -1,    91,    -1,
       8,     9,    -1,     8,    94,     9,    -1,    90,    -1,    94,
      90,    -1,    14,    96,     7,    -1,   120,    96,     7,    -1,
      97,    -1,    96,     5,    97,    -1,   136,    -1,   136,    98,
      -1,    47,    99,    -1,   101,    -1,   142,   100,    99,    -1,
      47,    -1,    48,    -1,    49,    -1,    50,    -1,    51,    -1,
      52,    -1,    53,    -1,    54,    -1,    55,    -1,    57,    -1,
      56,    -1,   154,    -1,   154,    59,    99,     6,    99,    -1,
       7,    -1,    91,     7,    -1,    15,    10,    91,    11,    90,
      -1,    15,    10,    91,    11,    90,    16,    90,    -1,    17,
      90,    18,    10,    91,    11,     7,    -1,    18,    10,    91,
      11,    90,    -1,    19,    10,    92,     7,    92,     7,    92,
      11,    90,    -1,    19,    10,    14,    96,     7,    92,     7,
      92,    11,    90,    -1,    20,    10,   142,    21,    91,    11,
      90,    -1,    20,    10,    14,    97,    21,    91,    11,    90,
      -1,    26,     7,    -1,    26,   136,     7,    -1,    24,     7,
      -1,    24,   136,     7,    -1,    28,     7,    -1,    28,    91,
       7,    -1,    22,    10,    91,    11,   110,    -1,     8,   111,
       9,    -1,     8,   111,   113,   111,     9,    -1,    -1,   112,
      -1,    23,    91,     6,    -1,    23,    91,     6,    94,    -1,
      25,     6,    -1,    25,     6,    94,    -1,   136,     6,    90,
      -1,    33,    91,    -1,    30,    93,   117,    -1,    30,    93,
     118,    -1,    30,    93,   117,   118,    -1,    31,    10,   136,
      11,    93,    -1,    32,    93,    -1,   120,    58,   136,    -1,
     136,    -1,   136,    -1,   120,    72,   120,    74,    -1,    27,
     136,    10,   123,    11,     8,   125,     9,    -1,   120,   119,
      10,   123,    11,     8,   125,     9,    -1,    27,    10,   123,
      11,     8,   125,     9,    -1,    27,   136,    10,   123,    11,
       8,   125,     9,    -1,    -1,   124,    -1,   136,    -1,   124,
       5,   136,    -1,    -1,    88,    -1,    29,    -1,   136,    -1,
      83,    -1,   127,    -1,   131,    -1,    10,    91,    11,    -1,
      12,    13,    -1,    12,   130,    13,    -1,    12,   128,    13,
      -1,    12,   128,     5,   129,    13,    -1,   129,    99,    -1,
     128,     5,   129,    99,    -1,    -1,   130,    -1,     5,    -1,
     130,     5,    -1,     8,     9,    -1,     8,   132,     9,    -1,
       8,   132,     5,     9,    -1,   133,    -1,   132,     5,   133,
      -1,   134,     6,    99,    -1,    80,   134,    10,    11,     8,
     125,     9,    -1,    81,   134,    10,   135,    11,     8,   125,
       9,    -1,    40,    -1,    44,    -1,    86,    -1,   136,    -1,
      40,    -1,   126,    -1,   122,    -1,   137,    12,    91,    13,
      -1,   137,    58,    40,    -1,    36,   137,   140,    -1,   137,
      -1,    36,   138,    -1,   137,   140,    -1,   139,   140,    -1,
     139,    12,    91,    13,    -1,   139,    58,    40,    -1,    10,
      11,    -1,    10,   141,    11,    -1,    99,    -1,   141,     5,
      99,    -1,   138,    -1,   139,    -1,   142,    -1,   142,    45,
      -1,   142,    46,    -1,   143,    -1,    34,   144,    -1,    45,
     144,    -1,    46,   144,    -1,    65,   144,    -1,    66,   144,
      -1,    70,   144,    -1,    71,   144,    -1,   144,    -1,   145,
      67,   144,    -1,   145,    68,   144,    -1,   145,    69,   144,
      -1,   145,    -1,   146,    65,   145,    -1,   146,    66,   145,
      -1,   146,    -1,   147,    76,   146,    -1,   147,    77,   146,
      -1,   147,    -1,   148,    72,   147,    -1,   148,    74,   147,
      -1,   148,    73,   147,    -1,   148,    75,   147,    -1,   148,
      35,   147,    -1,   148,    21,   147,    -1,   148,    -1,   149,
      78,   148,    -1,   149,    79,   148,    -1,   149,    -1,   150,
      60,   149,    -1,   150,    -1,   151,    64,   150,    -1,   151,
      -1,   152,    62,   151,    -1,   152,    -1,   153,    61,   152,
      -1,   153,    -1,   154,    63,   153,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    92,    92,    93,    94,    95,    99,   103,   104,   109,
     110,   111,   114,   116,   120,   121,   125,   126,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   146,   147,   150,   151,   155,   156,   160,   161,   165,
     168,   174,   179,   186,   189,   195,   201,   203,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   223,
     224,   228,   232,   236,   238,   243,   246,   249,   252,   255,
     257,   262,   265,   271,   274,   280,   282,   287,   293,   296,
     301,   303,   307,   308,   312,   313,   317,   321,   325,   328,
     331,   337,   341,   345,   346,   349,   352,   357,   360,   366,
     368,   373,   375,   379,   381,   385,   388,   394,   396,   399,
     402,   405,   408,   413,   416,   419,   422,   427,   428,   431,
     433,   437,   438,   442,   443,   444,   448,   449,   453,   454,
     455,   459,   460,   461,   465,   469,   474,   476,   478,   480,
     482,   487,   490,   495,   498,   501,   504,   510,   513,   519,
     521,   526,   530,   536,   537,   539,   544,   545,   547,   550,
     553,   556,   559,   562,   568,   569,   572,   575,   581,   582,
     585,   591,   592,   594,   599,   600,   602,   604,   606,   608,
     609,   613,   614,   616,   621,   622,   627,   628,   633,   634,
     639,   640,   644,   645
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF_WITHOUT_ELSE", "ELSE", "CAMMA",
  "COLON", "SEMICOLON", "LBRACE", "RBRACE", "LCBRACE", "RCBRACE",
  "LPARENTHESIS", "RPARENTHESIS", "Var", "If", "Else", "Do", "While",
  "For", "ForEach", "In", "Switch", "Case", "Break", "Default", "Continue",
  "Function", "Return", "This", "Try", "CATCH", "FINALLY", "Throw",
  "Typeof", "Instanceof", "New", "T_NULL", "T_TRUE", "T_FALSE",
  "IdentifierName", "HexIntegerLiteral", "IntegerLiteral", "FloatLiteral",
  "StringLiteral", "PLUSPLUS", "MINUSMINUS", "EQ_LET", "MUL_LET",
  "DIV_LET", "REM_LET", "ADD_LET", "SUB_LET", "LSHIFT_LET", "RSHIFT_LET",
  "AND_LET", "OR_LET", "XOR_LET", "DOT", "QUESTION", "AND", "LAND", "OR",
  "LOR", "XOR", "ADD", "SUB", "MUL", "DIV", "REM", "INV", "NOT", "LT",
  "LTE", "GT", "GTE", "LSHIFT", "RSHIFT", "EQEQ", "NEQ", "\"get\"",
  "\"set\"", "$accept", "Literal", "NullLiteral", "BooleanLiteral",
  "NumericLiteral", "Program", "SourceElements", "SourceElement",
  "Statement", "Expression", "Expressionopt", "Block", "StatementList",
  "VariableStatement", "VariableDeclarationList", "VariableDeclaration",
  "Initialiser", "AssignmentExpression", "AssignmentOperator",
  "ConditionalExpression", "EmptyStatement", "ExpressionStatement",
  "IfStatement", "IterationStatement", "ContinueStatement",
  "BreakStatement", "ReturnStatement", "SwitchStatement", "CaseBlock",
  "CaseClausesopt", "CaseClause", "DefaultClause", "LabelledStatement",
  "ThrowStatement", "TryStatement", "Catch", "Finally", "MethodName",
  "TypeName", "FunctionDeclaration", "FunctionExpression",
  "FormalParameterListopt", "FormalParameterList", "FunctionBody",
  "PrimaryExpression", "ArrayLiteral", "ElementList", "Elisionopt",
  "Elision", "ObjectLiteral", "PropertyNameAndValueList",
  "PropertyAssignment", "PropertyName", "PropertySetParameterList",
  "Identifier", "MemberExpression", "NewExpression", "CallExpression",
  "Arguments", "ArgumentList", "LeftHandSideExpression",
  "PostfixExpression", "UnaryExpression", "MultiplicativeExpression",
  "AdditiveExpression", "ShiftExpression", "RelationalExpression",
  "EqualityExpression", "BitwiseANDExpression", "BitwiseXORExpression",
  "BitwiseORExpression", "LogicalANDExpression", "LogicalORExpression", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    83,    83,    83,    84,    85,    85,    86,
      86,    86,    87,    87,    88,    88,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    92,    92,    93,    93,    94,    94,    95,
      95,    96,    96,    97,    97,    98,    99,    99,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   101,
     101,   102,   103,   104,   104,   105,   105,   105,   105,   105,
     105,   106,   106,   107,   107,   108,   108,   109,   110,   110,
     111,   111,   112,   112,   113,   113,   114,   115,   116,   116,
     116,   117,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   126,
     126,   126,   126,   127,   127,   127,   127,   128,   128,   129,
     129,   130,   130,   131,   131,   131,   132,   132,   133,   133,
     133,   134,   134,   134,   135,   136,   137,   137,   137,   137,
     137,   138,   138,   139,   139,   139,   139,   140,   140,   141,
     141,   142,   142,   143,   143,   143,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   145,   145,   146,   146,
     146,   147,   147,   147,   148,   148,   148,   148,   148,   148,
     148,   149,   149,   149,   150,   150,   151,   151,   152,   152,
     153,   153,   154,   154
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     2,     3,     1,     2,     3,
       3,     1,     3,     1,     2,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     1,     2,     5,     7,     7,     5,     9,    10,     7,
       8,     2,     3,     2,     3,     2,     3,     5,     3,     5,
       0,     1,     3,     4,     2,     3,     3,     2,     3,     3,
       4,     5,     2,     3,     1,     1,     4,     8,     8,     7,
       8,     0,     1,     1,     3,     0,     1,     1,     1,     1,
       1,     1,     3,     2,     3,     3,     5,     2,     4,     0,
       1,     1,     2,     2,     3,     4,     1,     3,     3,     7,
       8,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     1,     2,     2,     2,     4,     3,     2,     3,     1,
       3,     1,     1,     1,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      12,    61,     0,     0,   119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     6,     7,     8,   135,    11,     9,    10,     5,     0,
       0,     0,     0,     0,     0,   109,     2,     3,     4,     0,
      13,    14,    16,     0,    18,    19,    31,    46,    20,    21,
      22,    23,    24,    25,    26,    28,    27,    29,    30,     0,
      17,   137,   136,   110,   111,   108,   141,   151,   152,   153,
     156,   164,   168,   171,   174,   181,   184,   186,   188,   190,
     192,    59,    35,     0,   135,     5,     0,     0,     4,    37,
       0,     0,     0,   126,     0,     0,     0,   108,   121,   113,
       0,     0,   120,     0,    41,    43,     0,     0,     0,    33,
       0,     0,    73,     0,    71,     0,   101,     0,    75,     0,
       0,     0,    87,   153,   157,   141,   142,   158,   159,   160,
     161,   162,   163,     1,    15,     0,    62,     0,     0,     0,
       0,    43,     0,     0,     0,     0,   143,     0,     0,   144,
     154,   155,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    58,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,   132,   133,     0,
       0,    36,    38,     0,   124,     0,   123,   112,   119,   115,
     117,   122,   114,     0,    39,     0,    44,     0,     0,     0,
       0,    34,     0,     0,     0,     0,    74,    72,     0,   102,
     103,   101,    76,    35,     0,     0,    88,    89,   140,    32,
       0,    95,    40,   101,     0,    86,   147,   149,     0,     0,
     139,     0,   146,    47,   165,   166,   167,   169,   170,   172,
     173,   180,   179,   175,   177,   176,   178,   182,   183,   185,
     187,   189,   191,     0,   193,   101,     0,     0,   125,   127,
     128,     0,   120,    42,    45,     0,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    92,    90,    96,
       0,    93,     0,   148,   138,   145,     0,     0,     0,     0,
     134,   116,   118,    63,     0,    66,    33,     0,     0,     0,
      80,    77,   105,   104,     0,     0,     0,   150,    60,     0,
     105,     0,     0,     0,     0,    33,     0,     0,     0,     0,
      81,   106,     0,   105,     0,   105,   105,     0,   105,    64,
      65,    33,     0,     0,    69,     0,    78,     0,    80,    99,
       0,    91,     0,     0,   129,     0,     0,     0,    70,    82,
      84,     0,    97,    98,   100,   130,     0,    67,    83,    85,
      79,    68
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    35,    36,    37,    38,    39,   331,    41,    42,    43,
     212,    44,    90,    45,   138,   104,   206,    46,   163,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   311,   329,
     330,   348,    56,    57,    58,   226,   227,   139,    91,    60,
      61,   218,   219,   332,    62,    63,   100,   101,   102,    64,
      92,    93,    94,   299,    97,    66,    67,    68,   146,   238,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -276
static const yytype_int16 yypact[] =
{
     695,  -276,   390,   960,    41,    21,    33,   760,    66,    79,
     102,   134,    17,    25,    31,   800,  -276,    43,   960,   960,
    1000,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,   960,
     960,   960,   960,   960,   960,  -276,  -276,  -276,  -276,   132,
     695,  -276,  -276,    68,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,    -3,
    -276,  -276,  -276,  -276,  -276,     2,     6,  -276,     9,   393,
    -276,  -276,   124,    35,   112,    78,   128,    95,    83,   111,
     137,   127,   432,    31,   202,   206,   141,   141,   214,  -276,
     565,    -3,    51,  -276,   218,    38,   110,  -276,  -276,  -276,
      50,   960,    57,   151,  -276,   182,   960,   217,   960,   840,
     266,   960,  -276,   225,  -276,   231,    21,   229,  -276,   163,
     630,   183,   235,   171,  -276,     6,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,   960,  -276,    21,   189,   250,
     -24,    87,   760,   880,   960,   222,  -276,   960,   223,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,   960,   960,   960,   960,   960,   960,   960,
     960,   960,   960,   960,   960,   960,   960,   960,   960,   960,
     960,   960,   960,   960,   960,   254,  -276,  -276,  -276,   256,
     258,  -276,  -276,    44,  -276,   960,  -276,  -276,   264,  -276,
    -276,  -276,  -276,    21,  -276,   960,  -276,   122,   260,   135,
      21,   235,   265,    21,   252,   138,  -276,  -276,   268,   276,
    -276,    21,  -276,  -276,   261,    43,   251,  -276,  -276,  -276,
     125,  -276,  -276,    21,    21,  -276,  -276,  -276,   143,    90,
    -276,   117,  -276,  -276,  -276,  -276,  -276,   124,   124,    35,
      35,   112,   112,   112,   112,   112,   112,    78,    78,   128,
      95,    83,   111,   279,   137,    21,   271,    21,  -276,  -276,
    -276,   920,   282,  -276,  -276,   760,   960,   760,   197,   960,
     267,   960,   281,   283,    21,   286,    21,  -276,  -276,  -276,
     287,  -276,   960,  -276,  -276,  -276,   960,   289,   303,   302,
    -276,  -276,  -276,   298,   158,  -276,   960,   308,   960,   160,
     293,  -276,   695,  -276,   309,   310,   311,  -276,  -276,   315,
     695,   316,   760,   318,   322,   960,   161,   760,   960,    11,
    -276,   695,   321,   695,    43,   695,   695,   323,   695,  -276,
    -276,   960,   328,   760,  -276,   213,  -276,   334,   293,  -276,
     325,  -276,   332,   335,  -276,   337,   336,   760,  -276,   760,
     760,   340,   492,  -276,  -276,  -276,   760,  -276,   760,   760,
    -276,  -276
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -276,  -276,  -276,  -276,     7,  -276,   350,   -37,    15,    20,
    -275,   -16,  -137,  -276,     1,  -164,  -276,   -91,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,     4,
    -276,  -276,  -276,  -276,  -276,  -276,   129,  -276,    58,  -276,
    -276,  -188,  -276,  -197,  -276,  -276,  -276,   153,   155,  -276,
    -276,   169,   139,  -276,     0,   343,   344,  -276,   -57,  -276,
      77,  -276,    -4,    60,    61,     3,    59,   175,   176,   184,
     185,   187,  -276
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -134
static const yytype_int16 yytable[] =
{
      65,   121,    65,   134,   307,   105,   103,    65,   142,    88,
     200,   149,   113,   115,   117,   124,   143,    89,   144,   143,
     346,   147,   107,    96,   112,   127,   128,   129,   130,   131,
     132,   324,   114,   285,   234,   119,   347,    24,   122,   273,
      65,   116,   -95,   106,   229,   290,    98,   196,   137,   280,
     342,   120,   237,   268,    99,   198,   193,    24,    59,   141,
     194,    24,   201,   199,   145,    24,   356,   148,   228,   137,
     202,    24,   243,   135,   -95,   136,   108,   297,   186,    25,
      26,    27,   187,   185,   186,    25,    26,    27,   187,   109,
      65,   105,   263,   188,   188,   135,   123,   -94,    59,   171,
     167,   168,   188,   294,   270,   192,   123,   123,   123,   123,
     123,   123,   110,   172,   274,   135,   220,   140,    86,    87,
      65,   197,   135,   337,    86,    87,   207,   135,   209,   211,
     295,   215,   133,   275,   205,    89,   350,   231,   352,   353,
     135,   355,    65,   135,   111,   -95,   277,   180,   292,   282,
     173,   174,   175,   176,   293,   179,   203,   235,   204,   -95,
     244,   245,   246,   135,   239,   135,   135,   241,   135,   323,
     222,   327,   343,   181,   251,   252,   253,   254,   255,   256,
     302,   186,    25,    26,    27,   187,   183,   214,   169,   170,
     184,   164,   165,   166,   203,   230,   232,   137,   182,   289,
     188,   317,   203,   105,   306,   318,   177,   178,  -131,   287,
     105,   278,  -132,   105,   224,   225,   150,   151,   135,   359,
    -133,   220,   368,   369,   195,   189,   190,   247,   248,   205,
     249,   250,   216,   220,   291,   208,   257,   258,   217,   221,
     135,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     233,   123,   240,   242,   265,   220,   266,   300,   267,    98,
     276,   286,   279,   281,    95,    65,     3,    65,     4,   283,
     213,   284,   298,   225,   313,   296,   315,   201,   308,   310,
     303,   312,   305,    83,   134,    16,   304,   314,   316,   211,
     319,   309,    20,    21,    22,    23,    24,    25,    26,    27,
      28,   320,    65,   321,   322,   325,   328,   333,   351,   335,
      65,   334,    65,   336,   338,   340,   211,    65,   326,   341,
     349,    65,   354,    65,   362,    65,    65,   339,    65,   357,
     360,   363,   344,    65,   364,   211,   365,   366,   345,   370,
      40,   271,   361,   272,   259,   288,   260,    65,   358,    65,
      65,   211,   269,   125,   126,   261,    65,   262,    65,    65,
      59,   264,   367,     0,    89,    89,     0,     0,    59,     0,
       0,   371,     0,   192,   192,     0,     0,     0,     0,    59,
       0,    59,     0,    59,    59,     0,    59,     1,     2,    82,
       3,     0,     4,     0,     5,     6,     0,     7,     8,     9,
      10,     0,    11,     0,    12,     0,    13,    83,    15,    16,
      17,     0,     0,    18,    19,     0,    20,    21,    22,    23,
      84,    25,    26,    27,    85,    29,    30,  -123,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,  -123,     0,    31,    32,     0,     0,     0,
      33,    34,     0,     0,     0,     0,     0,  -123,     0,     0,
      86,    87,     0,     0,     0,     0,     0,     0,     0,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -100,     0,  -123,
    -123,  -123,     0,     0,  -123,  -123,  -123,  -123,  -123,  -123,
    -123,  -123,     0,  -100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -100,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -100,
    -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,  -100,
    -100,  -100,  -100,  -100,  -100,  -100,  -100,     0,     0,  -100,
    -100,  -100,     0,     0,  -100,  -100,  -100,  -100,  -100,  -100,
    -100,  -100,     1,     2,   191,     3,     0,     4,     0,     5,
       6,     0,     7,     8,     9,    10,     0,    11,     0,    12,
       0,    13,    83,    15,    16,    17,     0,     0,    18,    19,
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,     0,     0,     0,    33,    34,     1,     2,   223,
       3,     0,     4,     0,     5,     6,     0,     7,     8,     9,
      10,     0,    11,     0,    12,     0,    13,    83,    15,    16,
      17,     0,     0,    18,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,     0,     0,     0,
      33,    34,     1,     2,     0,     3,     0,     4,     0,     5,
       6,     0,     7,     8,     9,    10,     0,    11,     0,    12,
       0,    13,    14,    15,    16,    17,     0,     0,    18,    19,
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,     0,     0,     0,    33,    34,     1,     2,     0,
       3,     0,     4,     0,     5,     6,     0,     7,     8,     9,
      10,     0,    11,     0,    12,     0,    13,    83,    15,    16,
      17,     0,     0,    18,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   118,    95,     0,
       3,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    95,     0,
       3,     0,     4,     0,   210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    95,     0,
       3,   236,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    95,     0,
       3,     0,     4,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    95,     0,
       3,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,    19,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    95,     0,
       3,     0,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    83,     0,    16,
      33,    34,     0,     0,     0,     0,    20,    21,    22,    23,
      24,    25,    26,    27,    28
};

static const yytype_int16 yycheck[] =
{
       0,    17,     2,    40,   279,     5,     5,     7,     6,     2,
     101,    68,    12,    13,    14,    19,    10,     2,    12,    10,
       9,    12,     7,     3,     7,    29,    30,    31,    32,    33,
      34,   306,     7,   221,    58,    15,    25,    40,    18,   203,
      40,    10,    40,    10,   135,   233,     5,     9,    72,   213,
     325,     8,   143,     9,    13,     5,     5,    40,     0,    59,
       9,    40,     5,    13,    58,    40,   341,    58,   125,    72,
      13,    40,   163,     5,    72,     7,    10,   265,    40,    41,
      42,    43,    44,    83,    40,    41,    42,    43,    44,    10,
      90,    91,   183,    86,    87,     5,    19,    10,    40,    21,
      65,    66,    95,    13,   195,    90,    29,    30,    31,    32,
      33,    34,    10,    35,   205,     5,   116,    59,    80,    81,
     120,    11,     5,   320,    80,    81,   106,     5,   108,   109,
      13,   111,     0,    11,    47,   120,   333,   137,   335,   336,
       5,   338,   142,     5,    10,    58,    11,    64,     5,    11,
      72,    73,    74,    75,    11,    60,     5,   142,     7,    72,
     164,   165,   166,     5,   144,     5,     5,   147,     5,    11,
       7,    11,    11,    62,   171,   172,   173,   174,   175,   176,
     271,    40,    41,    42,    43,    44,    59,   110,    76,    77,
      63,    67,    68,    69,     5,   137,     7,    72,    61,    74,
     193,   292,     5,   203,     7,   296,    78,    79,     6,   225,
     210,   210,     6,   213,    31,    32,    45,    46,     5,     6,
       6,   221,   359,   360,     6,    86,    87,   167,   168,    47,
     169,   170,     7,   233,   234,    18,   177,   178,     7,    10,
       5,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
      10,   184,    40,    40,    10,   265,    10,   267,    10,     5,
      10,    10,     7,    21,     8,   275,    10,   277,    12,    11,
      14,     5,    11,    32,   284,     6,   286,     5,    21,     8,
     275,     8,   277,    27,   331,    29,   276,    11,    11,   279,
      11,   281,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     8,   312,    11,    16,     7,    23,     8,   334,     8,
     320,    11,   322,     8,     8,     7,   306,   327,   308,     7,
       9,   331,     9,   333,     9,   335,   336,   322,   338,    11,
       6,     9,   327,   343,     9,   325,     9,    11,   328,     9,
       0,   198,   348,   198,   179,   226,   180,   357,   343,   359,
     360,   341,   193,    20,    20,   181,   366,   182,   368,   369,
     312,   184,   357,    -1,   359,   360,    -1,    -1,   320,    -1,
      -1,   366,    -1,   368,   369,    -1,    -1,    -1,    -1,   331,
      -1,   333,    -1,   335,   336,    -1,   338,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    24,    -1,    26,    27,    28,    29,
      30,    -1,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,     5,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    21,    -1,    65,    66,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     5,    -1,    67,
      68,    69,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    -1,    67,
      68,    69,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,     7,     8,     9,    10,    -1,    12,    -1,    14,
      15,    -1,    17,    18,    19,    20,    -1,    22,    -1,    24,
      -1,    26,    27,    28,    29,    30,    -1,    -1,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    70,    71,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    24,    -1,    26,    27,    28,    29,
      30,    -1,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      70,    71,     7,     8,    -1,    10,    -1,    12,    -1,    14,
      15,    -1,    17,    18,    19,    20,    -1,    22,    -1,    24,
      -1,    26,    27,    28,    29,    30,    -1,    -1,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    70,    71,     7,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    24,    -1,    26,    27,    28,    29,
      30,    -1,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,     7,     8,    -1,
      10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,     8,    -1,
      10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,     8,    -1,
      10,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,     8,    -1,
      10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,     8,    -1,
      10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    27,    -1,    29,
      70,    71,    -1,    -1,    -1,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,     8,    10,    12,    14,    15,    17,    18,    19,
      20,    22,    24,    26,    27,    28,    29,    30,    33,    34,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    65,    66,    70,    71,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    95,    99,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   114,   115,   116,   120,
     121,   122,   126,   127,   131,   136,   137,   138,   139,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,     9,    27,    40,    44,    80,    81,    86,    90,
      94,   120,   132,   133,   134,     8,    91,   136,     5,    13,
     128,   129,   130,    96,    97,   136,    10,    90,    10,    10,
      10,    10,     7,   136,     7,   136,    10,   136,     7,    91,
       8,    93,    91,   142,   144,   137,   138,   144,   144,   144,
     144,   144,   144,     0,    89,     5,     7,    72,    96,   119,
     120,   136,     6,    10,    12,    58,   140,    12,    58,   140,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,   100,    67,    68,    69,    65,    66,    76,
      77,    21,    35,    72,    73,    74,    75,    78,    79,    60,
      64,    62,    61,    59,    63,   136,    40,    44,    86,   134,
     134,     9,    90,     5,     9,     6,     9,    11,     5,    13,
      99,     5,    13,     5,     7,    47,    98,    91,    18,    91,
      14,    91,    92,    14,   142,    91,     7,     7,   123,   124,
     136,    10,     7,     9,    31,    32,   117,   118,   140,    99,
     120,   136,     7,    10,    58,    90,    11,    99,   141,    91,
      40,    91,    40,    99,   144,   144,   144,   145,   145,   146,
     146,   147,   147,   147,   147,   147,   147,   148,   148,   149,
     150,   151,   152,    99,   153,    10,    10,    10,     9,   133,
      99,   129,   130,    97,    99,    11,    10,    11,    96,     7,
      97,    21,    11,    11,     5,   123,    10,    93,   118,    74,
     123,   136,     5,    11,    13,    13,     6,   123,    11,   135,
     136,    13,    99,    90,    91,    90,     7,    92,    21,    91,
       8,   110,     8,   136,    11,   136,    11,    99,    99,    11,
       8,    11,    16,    11,    92,     7,    91,    11,    23,   111,
     112,    88,   125,     8,    11,     8,     8,   125,     8,    90,
       7,     7,    92,    11,    90,    91,     9,    25,   113,     9,
     125,    93,   125,   125,     9,   125,    92,    11,    90,     6,
       6,   111,     9,     9,     9,     9,    11,    90,    94,    94,
       9,    90
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

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
       `yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
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
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 33:

/* Line 1455 of yacc.c  */
#line 150 "konoha-paser.y"
    {;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 151 "konoha-paser.y"
    {;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 155 "konoha-paser.y"
    {;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 156 "konoha-paser.y"
    {;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 165 "konoha-paser.y"
    {
        (yyval.token) = variable_decl(ctx, NULL, (yyvsp[(2) - (3)].vec));
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 168 "konoha-paser.y"
    {
        (yyval.token) = variable_decl(ctx, (yyvsp[(1) - (3)].token), (yyvsp[(2) - (3)].vec));
    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 174 "konoha-paser.y"
    {
        Array(Token) *a = Array_new(Token);
        Array_add(Token, a, (yyvsp[(1) - (1)].token));
        (yyval.vec) = a;
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 179 "konoha-paser.y"
    {
        Array_add(Token, (yyvsp[(1) - (3)].vec), (yyvsp[(3) - (3)].token));
        (yyval.vec) = (yyvsp[(1) - (3)].vec);
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 186 "konoha-paser.y"
    {
        (yyval.token) = build_variable_decl((yyvsp[(1) - (1)].token), NULL);
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 189 "konoha-paser.y"
    {
        (yyval.token) = build_variable_decl((yyvsp[(1) - (2)].token), (yyvsp[(2) - (2)].token));
    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 195 "konoha-paser.y"
    {
        (yyval.token) = (yyvsp[(2) - (2)].token);
    ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 201 "konoha-paser.y"
    {
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 203 "konoha-paser.y"
    {
        (yyval.token) = build_assignment_expr((yyvsp[(2) - (3)].op), (yyvsp[(1) - (3)].token), (yyvsp[(3) - (3)].token));
    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 209 "konoha-paser.y"
    { (yyval.op) = OpEQLET ; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 210 "konoha-paser.y"
    { (yyval.op) = OpMULLET; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 211 "konoha-paser.y"
    { (yyval.op) = OpDIVLET; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 212 "konoha-paser.y"
    { (yyval.op) = OpMODLET; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 213 "konoha-paser.y"
    { (yyval.op) = OpADDLET; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 214 "konoha-paser.y"
    { (yyval.op) = OpSUBLET; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 215 "konoha-paser.y"
    { (yyval.op) = OpLSFTLET; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 216 "konoha-paser.y"
    { (yyval.op) = OpRSFTLET; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 217 "konoha-paser.y"
    { (yyval.op) = OpANDLET; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 218 "konoha-paser.y"
    { (yyval.op) = OpXORLET; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 219 "konoha-paser.y"
    { (yyval.op) = OpORLET;  ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 228 "konoha-paser.y"
    {;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 236 "konoha-paser.y"
    {
    ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 238 "konoha-paser.y"
    {
    ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 243 "konoha-paser.y"
    {
        /*debug("While1");*/
    ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 246 "konoha-paser.y"
    {
        /*debug("While2");*/
    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 249 "konoha-paser.y"
    {
        /*debug("for1");*/
    ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 252 "konoha-paser.y"
    {
        /*debug("for2");*/
    ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 255 "konoha-paser.y"
    {
    ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 257 "konoha-paser.y"
    {
    ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 262 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 265 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 271 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 274 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 280 "konoha-paser.y"
    {
    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 282 "konoha-paser.y"
    {
    ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 287 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 293 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 296 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 325 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 328 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 331 "konoha-paser.y"
    {
        TODO();
    ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 349 "konoha-paser.y"
    {
        (yyval.token) = knh_Token_toType((yyvsp[(1) - (1)].token));
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 352 "konoha-paser.y"
    {
        debug("todo: generics");
    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 357 "konoha-paser.y"
    {
        debug("FunctionDeclaration");
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 360 "konoha-paser.y"
    {
        debug("FunctionDeclaration");
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 366 "konoha-paser.y"
    {
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 368 "konoha-paser.y"
    {
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 373 "konoha-paser.y"
    {
    ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 379 "konoha-paser.y"
    {
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 381 "konoha-paser.y"
    {
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 385 "konoha-paser.y"
    {
        debug("FunctionBody1");
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 388 "konoha-paser.y"
    {
        debug("FunctionBody2");
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 394 "konoha-paser.y"
    {
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 396 "konoha-paser.y"
    {
        /*debug("PrimaryExpression2");*/
    ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 399 "konoha-paser.y"
    {
        /*debug("PrimaryExpression3");*/
    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 402 "konoha-paser.y"
    {
        /*debug("PrimaryExpression4");*/
    ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 405 "konoha-paser.y"
    {
        /*debug("PrimaryExpression5");*/
    ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 408 "konoha-paser.y"
    {
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 413 "konoha-paser.y"
    {
        /* () */
    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 416 "konoha-paser.y"
    {
        /* (,) (a,) */
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 419 "konoha-paser.y"
    {
        /* (a=1, v=3) */
    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 422 "konoha-paser.y"
    {
    ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 469 "konoha-paser.y"
    {
    ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 474 "konoha-paser.y"
    {
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 476 "konoha-paser.y"
    {
    ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 478 "konoha-paser.y"
    {
    ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 480 "konoha-paser.y"
    {
    ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 482 "konoha-paser.y"
    {
    ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 487 "konoha-paser.y"
    {
        /*debug("NewExpression1");*/
    ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 490 "konoha-paser.y"
    {
    ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 495 "konoha-paser.y"
    {
        debug("CallExpression1");
    ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 498 "konoha-paser.y"
    {
        debug("CallExpression2");
    ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 501 "konoha-paser.y"
    {
        debug("CallExpression3");
    ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 504 "konoha-paser.y"
    {
        debug("CallExpression4");
    ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 510 "konoha-paser.y"
    {
        debug("Arguments1");
    ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 513 "konoha-paser.y"
    {
        debug("Arguments2");
    ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 519 "konoha-paser.y"
    {
    ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 521 "konoha-paser.y"
    {
    ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 526 "konoha-paser.y"
    {
        /*debug("LeftHandSideExpression1");*/
        /*$$ = js_build_propaty_expr($1);*/
    ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 530 "konoha-paser.y"
    {
        /*debug("LeftHandSideExpression2");*/
    ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 536 "konoha-paser.y"
    {;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 537 "konoha-paser.y"
    {
    ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 539 "konoha-paser.y"
    {
    ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 544 "konoha-paser.y"
    {;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 545 "konoha-paser.y"
    {
    ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 547 "konoha-paser.y"
    {
        debug("++ UnaryExpression");
    ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 550 "konoha-paser.y"
    {
        debug("-- UnaryExpression");
    ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 553 "konoha-paser.y"
    {
        debug("+ val");
    ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 556 "konoha-paser.y"
    {
        debug("- val");
    ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 559 "konoha-paser.y"
    {
        debug("INV val");
    ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 562 "konoha-paser.y"
    {
        debug("NOT UnaryExpression");
    ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 569 "konoha-paser.y"
    {
        debug("MultiplicativeExpression1");
    ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 572 "konoha-paser.y"
    {
        debug0("MultiplicativeExpression2");
    ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 575 "konoha-paser.y"
    {
        debug0("MultiplicativeExpression3");
    ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 582 "konoha-paser.y"
    {
        debug0("AdditiveExpression1");
    ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 585 "konoha-paser.y"
    {
        debug0("AdditiveExpression2");
    ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 592 "konoha-paser.y"
    {
    ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 594 "konoha-paser.y"
    {
    ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 600 "konoha-paser.y"
    {
    ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 602 "konoha-paser.y"
    {
    ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 604 "konoha-paser.y"
    {
    ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 606 "konoha-paser.y"
    {
    ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 614 "konoha-paser.y"
    {
    ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 616 "konoha-paser.y"
    {
    ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 622 "konoha-paser.y"
    {
    ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 628 "konoha-paser.y"
    {
    ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 634 "konoha-paser.y"
    {
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2917 "build/y.gen.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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



