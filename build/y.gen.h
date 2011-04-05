
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

/* Line 1676 of yacc.c  */
#line 27 "konoha-paser.y"

  Array(Token) *vec;
  knh_Token_t *token;
  KOperator op;



/* Line 1676 of yacc.c  */
#line 139 "build/y.gen.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE konoha_lval;

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

extern YYLTYPE konoha_lloc;

