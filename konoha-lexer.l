%{

#if 0
#define YYDEBUG 1
#endif
#include "konoha.h"
#include "y.gen.h"

int konoha_wrap (void);
int konoha_error (const char *);

#ifndef YY_DECL
#define YY_DECL int konoha_lex(CTX ctx)
#endif
extern YY_DECL;
static void write_buf(char *str);
static char *buf_tochar(void);
//static void konoha_unput (int, register char *);
//static int input (void);

knh_Token_t *konoha_build_id(const char *);
knh_Token_t *konoha_build_int(int);
knh_Token_t *konoha_build_float(float val);
knh_Token_t *konoha_build_string(const char *);
void knh_dump(CTX ctx, knh_Object_t *o);

%}

%pointer
%option yylineno

%start COMMENT MULTI_COMMENT DoubleStringCharacters SingleStringCharacters

%%
<INITIAL>"null"       {
    konoha_lval.token = NULL;
    return T_NULL;
}
<INITIAL>"true"       {
    //konoha_lval.token = boolean_true_node;
    return T_TRUE;
}
<INITIAL>"false"      {
    //konoha_lval.token = boolean_false_node;
    return T_FALSE;
}

<INITIAL>"var"        return Var;
<INITIAL>"if"         return If;
<INITIAL>"else"       {
    return Else;
}
<INITIAL>"do"         return Do;
<INITIAL>"while"      return While;
<INITIAL>"return"     return Return;
<INITIAL>"for"        return For;
<INITIAL>"in"         return In;
<INITIAL>"new"        return New;
<INITIAL>"this"       return This;
<INITIAL>"typeof"     return Typeof;
<INITIAL>"case"       return Case;
<INITIAL>"continue"   return Continue;
<INITIAL>"break"      return Break;
<INITIAL>"default"    return Default;
<INITIAL>"throw"      return Throw;
<INITIAL>"switch"     return Switch;
<INITIAL>"function"   return Function;
<INITIAL>"try"        return Try;
<INITIAL>"catch"      return CATCH;
<INITIAL>"finally"    return FINALLY;
<INITIAL>"instanceof" return Instanceof;

<INITIAL>","       return CAMMA;
<INITIAL>"."       return DOT;
<INITIAL>"+"       return ADD;
<INITIAL>"-"       return SUB;
<INITIAL>"*"       return MUL;
<INITIAL>"/"       return DIV;
<INITIAL>"%"       return REM;

<INITIAL>"~"       return INV;
<INITIAL>"!"       return NOT;
<INITIAL>"<<"     return LSHIFT;
<INITIAL>">>"     return RSHIFT;
<INITIAL>"<="      return LTE;
<INITIAL>"<"       return LT;
<INITIAL>">="      return GTE;
<INITIAL>">"       return GT;
<INITIAL>":"       return COLON;
<INITIAL>";"       return SEMICOLON;
<INITIAL>"{"       return LBRACE;
<INITIAL>"}"       return RBRACE;
<INITIAL>"("       return LCBRACE;
<INITIAL>")"       return RCBRACE;
<INITIAL>"["       return LPARENTHESIS;
<INITIAL>"]"       return RPARENTHESIS;
<INITIAL>"++"      return PLUSPLUS;
<INITIAL>"--"      return MINUSMINUS;
<INITIAL>"=="      return EQEQ;     /* ==  */
<INITIAL>"!="      return NEQ;      /* !=  */
<INITIAL>"="       return EQ_LET;     /* = */
<INITIAL>"*="      return MUL_LET;    /* *= */
<INITIAL>"/="      return DIV_LET;    /* /= */
<INITIAL>"%="      return REM_LET;    /* %= */
<INITIAL>"+="      return ADD_LET;    /* += */
<INITIAL>"-="      return SUB_LET;    /* -= */
<INITIAL>"<<="     return LSHIFT_LET; /* <<=  */
<INITIAL>">>="     return RSHIFT_LET; /* >>=  */
<INITIAL>"&="      return AND_LET;    /* &= */
<INITIAL>"^="      return XOR_LET;    /* ^= */
<INITIAL>"|="      return OR_LET;     /* |= */

<INITIAL>[ \t] ;
<INITIAL>[ \t\r\n] {}

<INITIAL>\" {
    BEGIN DoubleStringCharacters;
}
<INITIAL>\' {
    BEGIN SingleStringCharacters;
}
<INITIAL>"//"  BEGIN COMMENT;
<COMMENT>. ;
<COMMENT>\n  {
    BEGIN INITIAL;
}
<INITIAL>"/*"  BEGIN MULTI_COMMENT;
<MULTI_COMMENT>. ;
<MULTI_COMMENT>\n  {}
<MULTI_COMMENT>"*/"  {
    BEGIN INITIAL;
}
<INITIAL>[A-Za-z_$][A-Za-z_0-9]* {
    konoha_lval.token = konoha_build_id(yytext);
    knh_dump(ctx, cast(knh_Object_t*, konoha_lval.token));
    return IdentifierName;
}
<INITIAL>[1-9][0-9]* {
    int temp;
    sscanf(yytext, "%d", &temp);
    konoha_lval.token = konoha_build_int(temp);
    return IntegerLiteral;
}
<INITIAL>(0[Xx])?[0-9a-zA-Z]+ {
    int temp;
    sscanf(yytext, "%d", &temp);
    konoha_lval.token = konoha_build_int(temp);
    return HexIntegerLiteral;
}
<INITIAL>[-+]?[0-9]+(\.[0-9]*)([eE][-+]?[0-9]+)? {
    float temp;
    sscanf(yytext, "%g", &temp);
    konoha_lval.token = konoha_build_float(temp);
    return FloatLiteral;
}

<DoubleStringCharacters>\" {
    BEGIN INITIAL;
    konoha_lval.token = konoha_build_string(buf_tochar());
    return StringLiteral;
}
<DoubleStringCharacters>\n  {
    konoha_error("terminate string without '\"'");
}
<DoubleStringCharacters>\\\" {
    /* '"'  */
    write_buf(yytext);
}
<DoubleStringCharacters>\\n  {
    /* '\n' */
    write_buf(yytext);
}
<DoubleStringCharacters>\\t {
    /* '\t' */
    write_buf(yytext);
}
<DoubleStringCharacters>\\\\  {
    /* '\\' */
    write_buf(yytext);
}
<DoubleStringCharacters>.  {
    /*  */
    write_buf(yytext);
}

<SingleStringCharacters>\' {
    BEGIN INITIAL;
    konoha_lval.token = konoha_build_string(buf_tochar());
    return StringLiteral;
}
<SingleStringCharacters>\n  {
    konoha_error("terminate string without '\''");
}
<SingleStringCharacters>\\\' {
    /* '"'  */
    write_buf(yytext);
}
<SingleStringCharacters>\\n  {
    /* '\n' */
    write_buf(yytext);
}
<SingleStringCharacters>\\t  {
    /* '\t' */
    write_buf(yytext);
}
<SingleStringCharacters>\\\\ {
    /* '\\' */
    write_buf(yytext);
}
<SingleStringCharacters>.    {
    /*  */
    write_buf(yytext);
}

<INITIAL>. {
    fprintf(stderr, "lexical error\n");
    exit(1);
}
%%
/*print as a keyword*/ 
static void write_buf(char*);
static char *buf_tochar(void)
{
  int len;
  char *str;
  return str;
}
static void write_buf(char *str)
{
}

int konoha_wrap (void)
{
  return 1;
}

int konoha_error (const char *msg)
{
  error("%s", msg);
  exit(1);
}
