%{

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

%}

%union
{
  Array(Token) *vec;
  knh_Token_t *token;
  KOperator op;
}

%error-verbose
%debug

/* Locations will be added later to add debugging information */
%locations


%start Program

%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE
%token CAMMA
%token COLON
%token SEMICOLON
%token LBRACE RBRACE   /* { } */
%token LCBRACE RCBRACE /* ( ) */
%token LPARENTHESIS RPARENTHESIS /* [ ] */
%token Var
%token If Else 
%token Do While 
%token For ForEach
%token In 
%token Switch Case Break Default 
%token Continue
%token Function Return 
%token This
%token Try CATCH FINALLY Throw 
%token Typeof Instanceof
%token New
%token T_NULL T_TRUE T_FALSE
%token IdentifierName
%token HexIntegerLiteral IntegerLiteral FloatLiteral StringLiteral
%token PLUSPLUS MINUSMINUS
%token EQ_LET                   /* = */
%token MUL_LET DIV_LET REM_LET  /* *= /= %= */
%token ADD_LET SUB_LET          /* += -= */
%token LSHIFT_LET RSHIFT_LET    /* <<= >>= */
%token AND_LET OR_LET XOR_LET   /* &= |= ^=*/
%token DOT QUESTION             /* . ? */
%token AND LAND OR LOR XOR      /* & && | || ^ */
%token ADD SUB MUL DIV REM      /* + - * / % */
%token INV NOT LT LTE GT GTE    /* ~ ! < <= > >= */
%token LSHIFT RSHIFT            /* << >> */
%token EQEQ NEQ                 /* == != */

%type<token> Identifier TypeName
%type<token> Initialiser Expression
%type<token> VariableDeclaration
%type<token> AssignmentExpression ConditionalExpression
%type<token> StatementList Statement
%type<token> Block VariableStatement EmptyStatement
%type<token> ExpressionStatement IfStatement
%type<token> IterationStatement ContinueStatement
%type<token> BreakStatement ReturnStatement
%type<token> LabelledStatement SwitchStatement
%type<token> ThrowStatement TryStatement
%type<token> Catch Finally
%type<token> Expressionopt

%type<vec> Arguments ArgumentList VariableDeclarationList
%type<op>  AssignmentOperator

%type<token> LeftHandSideExpression NewExpression CallExpression
%type<token> PostfixExpression UnaryExpression
%type<token> MultiplicativeExpression AdditiveExpression
%type<token> ShiftExpression RelationalExpression
%type<token> EqualityExpression

%type<token> LogicalORExpression LogicalANDExpression
%type<token> BitwiseANDExpression BitwiseXORExpression BitwiseORExpression
%%
Literal 
    : NullLiteral
    | BooleanLiteral
    | NumericLiteral
    | StringLiteral
    ;

NullLiteral 
    : T_NULL
    ;

BooleanLiteral 
    : T_TRUE
    | T_FALSE
    ;

/* DecimalLiteral */
NumericLiteral
    : IntegerLiteral
    | FloatLiteral
    | HexIntegerLiteral;


Program 
    : /* not in spec */ 
    | SourceElements 
;

SourceElements
    : SourceElement
    | SourceElements SourceElement
;

SourceElement 
    : Statement
    | FunctionDeclaration
;

Statement
    : Block
    | VariableStatement
    | EmptyStatement
    | ExpressionStatement
    | IfStatement
    | IterationStatement
    | ContinueStatement
    | BreakStatement
    | ReturnStatement
    | LabelledStatement
    | SwitchStatement
    | ThrowStatement
    | TryStatement
    ;

Expression 
    : AssignmentExpression 
    | Expression CAMMA AssignmentExpression ;

Expressionopt
    : {}
    | Expression {}
    ;

Block 
    : LBRACE RBRACE {}
    | LBRACE StatementList RBRACE {}
    ;

StatementList 
    : Statement {
        knh_Token_t *stmts = new_TokenStmtList();
        knh_TokenStmtList_add(stmts, $1);
        $$ = stmts;
    }
    | StatementList Statement {
        knh_Token_t *stmts = $1;
        knh_TokenStmtList_add(stmts, $2);
    }
    ;

VariableStatement 
    : Var VariableDeclarationList SEMICOLON {
        $$ = variable_decl(ctx, NULL, $2);
    }
    | TypeName VariableDeclarationList SEMICOLON {
        $$ = variable_decl(ctx, $1, $2);
    }
    ;

VariableDeclarationList 
    : VariableDeclaration {
        Array(Token) *a = Array_new(Token);
        Array_add(Token, a, $1);
        $$ = a;
    }
    | VariableDeclarationList CAMMA VariableDeclaration {
        Array_add(Token, $1, $3);
        $$ = $1;
    }
    ;

VariableDeclaration 
    : Identifier {
        $$ = build_variable_decl($1, NULL);
    }
    | Identifier Initialiser {
        $$ = build_variable_decl($1, $2);
    }
    ;

Initialiser 
    : EQ_LET AssignmentExpression {
        $$ = $2;
    }
    ;

AssignmentExpression 
    : ConditionalExpression {
    }
    | LeftHandSideExpression AssignmentOperator AssignmentExpression {
        $$ = build_assignment_expr($2, $1, $3);
    }
    ;

AssignmentOperator
    : EQ_LET     /*    = */ { $$ = OpEQLET ; }
    | MUL_LET    /*   *= */ { $$ = OpMULLET; }
    | DIV_LET    /*   /= */ { $$ = OpDIVLET; }
    | REM_LET    /*   %= */ { $$ = OpMODLET; }
    | ADD_LET    /*   += */ { $$ = OpADDLET; }
    | SUB_LET    /*   -= */ { $$ = OpSUBLET; }
    | LSHIFT_LET /*  <<= */ { $$ = OpLSFTLET; }
    | RSHIFT_LET /*  >>= */ { $$ = OpRSFTLET; }
    | AND_LET    /*   &= */ { $$ = OpANDLET; }
    | XOR_LET    /*   ^= */ { $$ = OpXORLET; }
    | OR_LET     /*   |= */ { $$ = OpORLET;  }
    ;

ConditionalExpression 
    : LogicalORExpression
    | LogicalORExpression QUESTION AssignmentExpression COLON AssignmentExpression
    ;

EmptyStatement 
    : SEMICOLON {}
    ;

ExpressionStatement 
    : Expression SEMICOLON 
    ;

IfStatement 
    : If LCBRACE Expression RCBRACE Statement %prec IF_WITHOUT_ELSE {
        $$ = build_if_else($3, $5, NULL);
    }
    | If LCBRACE Expression RCBRACE Statement Else Statement {
        $$ = build_if_else($3, $5, $7);
    }
;

IterationStatement
    : Do Statement While LCBRACE Expression RCBRACE SEMICOLON {
        /*debug("While1");*/
        $$ = build_while($5, $2);
    }
    | While LCBRACE Expression RCBRACE Statement {
        /*debug("While2");*/
        $$ = build_while($3, $5);
    }
    | For LCBRACE Expressionopt SEMICOLON Expressionopt SEMICOLON Expressionopt RCBRACE Statement {
        /*debug("for1");*/
        $$ = build_for($3, $5, $7, $9);

    }
    | For LCBRACE TypeName VariableDeclarationList SEMICOLON Expressionopt SEMICOLON Expressionopt RCBRACE Statement {
        /*debug("for2");*/
        $$ = build_for_decl($3, $4, $6, $8, $10);
    }
    | ForEach LCBRACE LeftHandSideExpression In Expression RCBRACE Statement {
        $$ = build_foreach(NULL, $3, $5, $7);
    }
    | ForEach LCBRACE TypeName VariableDeclaration In Expression RCBRACE Statement {
        $$ = build_foreach($3, $4, $6, $8);
    }
;

ContinueStatement 
    : Continue SEMICOLON {
        TODO();
    }
    | Continue Identifier SEMICOLON {
        TODO();
    }
;

BreakStatement 
    : Break SEMICOLON {
        TODO();
    }
    | Break Identifier SEMICOLON {
        TODO();
    }
;

ReturnStatement 
    : Return SEMICOLON {
        $$ = build_reutrn(NULL);
    }
    | Return Expression SEMICOLON {
        $$ = build_reutrn($2);
    }
;

SwitchStatement 
    : Switch LCBRACE Expression RCBRACE CaseBlock {
        TODO();
    }
;

CaseBlock 
    : LBRACE CaseClausesopt RBRACE {
        TODO();
    }
    | LBRACE CaseClausesopt DefaultClause CaseClausesopt RBRACE {
        TODO();
    }
    ;

CaseClausesopt 
    :
    | CaseClause
;

CaseClause 
    : Case Expression COLON 
    | Case Expression COLON StatementList
;

DefaultClause
    : Default COLON
    | Default COLON StatementList
;

LabelledStatement 
    : Identifier COLON Statement
    ;

ThrowStatement 
    : Throw Expression {
        $$ = build_throw_expr($2);
    }
    ;

TryStatement 
    : Try Block Catch {
        $$ = build_try_catch($2, $3, NULL);
    }
    | Try Block Finally {
        $$ = build_try_catch($2, NULL, $3);
    }
    | Try Block Catch Finally {
        $$ = build_try_catch($2, $3, $4);
    }
    ;

Catch
    : CATCH LCBRACE Identifier RCBRACE Block {
        $$ = build_catch($3, $5);
    }
    ;

Finally
    : FINALLY Block {
        $$ = build_final($2);
    }
    ;

MethodName
    : TypeName DOT Identifier
    | Identifier

TypeName
    : Identifier {
        $$ = knh_Token_toType($1);
    }
    | TypeName LT TypeName GT {
        debug("todo: generics");
    }

FunctionDeclaration
    : Function Identifier LCBRACE FormalParameterListopt RCBRACE LBRACE FunctionBody RBRACE {
        debug("FunctionDeclaration");
    }
    | TypeName MethodName LCBRACE FormalParameterListopt RCBRACE LBRACE FunctionBody RBRACE {
        debug("FunctionDeclaration");
    }
    ;

FunctionExpression
    :Function LCBRACE FormalParameterListopt RCBRACE LBRACE FunctionBody RBRACE {
    }
    |Function Identifier LCBRACE FormalParameterListopt RCBRACE LBRACE FunctionBody RBRACE {
    }
    ;

FormalParameterListopt
    : {
    }
    | FormalParameterList
    ;

FormalParameterList
    : Identifier {
    }
    | FormalParameterList CAMMA Identifier {
    }

FunctionBody 
    : {
        debug("FunctionBody1");
    }
    |SourceElements {
        debug("FunctionBody2");
    }
    ;

PrimaryExpression 
    : This {
    }
    | Identifier {
        /*debug("PrimaryExpression2");*/
    }
    | Literal {
        /*debug("PrimaryExpression3");*/
    }
    | ArrayLiteral {
        /*debug("PrimaryExpression4");*/
    }
    | ObjectLiteral {
        /*debug("PrimaryExpression5");*/
    }
    | LCBRACE Expression RCBRACE {
    }
    ;

ArrayLiteral 
    : LPARENTHESIS RPARENTHESIS {
        /* () */
    }
    | LPARENTHESIS Elision RPARENTHESIS {
        /* (,) (a,) */
    }
    | LPARENTHESIS ElementList RPARENTHESIS {
        /* (a=1, v=3) */
    }
    | LPARENTHESIS ElementList CAMMA Elisionopt RPARENTHESIS {
    }
;

ElementList 
    : Elisionopt AssignmentExpression
    | ElementList CAMMA Elisionopt AssignmentExpression
    ;

Elisionopt
    :
    | Elision
    ;

Elision 
    : CAMMA
    | Elision CAMMA
    ;

ObjectLiteral 
    : LBRACE RBRACE
    | LBRACE PropertyNameAndValueList RBRACE
    | LBRACE PropertyNameAndValueList CAMMA RBRACE
    ;

PropertyNameAndValueList 
    : PropertyAssignment
    | PropertyNameAndValueList CAMMA PropertyAssignment
    ;

PropertyAssignment 
    : PropertyName COLON AssignmentExpression
    | "get" PropertyName LCBRACE RCBRACE LBRACE FunctionBody RBRACE
    | "set" PropertyName LCBRACE PropertySetParameterList RCBRACE LBRACE FunctionBody RBRACE
    ;

PropertyName 
    : IdentifierName
    | StringLiteral
    | NumericLiteral
    ;

PropertySetParameterList 
    : Identifier
    ;

Identifier 
    : IdentifierName {
    }
    ;

MemberExpression 
    : PrimaryExpression {
    }
    | FunctionExpression {
    }
    | MemberExpression LPARENTHESIS Expression RPARENTHESIS {
    }
    | MemberExpression DOT IdentifierName {
    }
    | New MemberExpression Arguments {
    }
    ;

NewExpression 
    : MemberExpression {
        /*debug("NewExpression1");*/
    }
    | New NewExpression {
    }
    ;

CallExpression 
    : MemberExpression Arguments {
        debug("CallExpression1");
    }
    | CallExpression Arguments {
        debug("CallExpression2");
    }
    | CallExpression LPARENTHESIS Expression RPARENTHESIS {
        debug("CallExpression3");
    }
    | CallExpression DOT IdentifierName {
        debug("CallExpression4");
    }
    ;

Arguments 
    : LCBRACE RCBRACE {
        debug("Arguments1");
        $$ = Array_new(Token);
    }
    | LCBRACE ArgumentList RCBRACE {
        debug("Arguments2");
        $$ = $2;
    }
    ;

ArgumentList 
    : AssignmentExpression {
        $$ = Array_new(Token);
    }
    | ArgumentList CAMMA AssignmentExpression {
        Array_add(Token, $1, $3);
    }
    ;

LeftHandSideExpression 
    : NewExpression
    | CallExpression
    ;

PostfixExpression
    : LeftHandSideExpression {}
    | LeftHandSideExpression PLUSPLUS {
        $$ = build_operator1(OpINC, $1);
    }
    | LeftHandSideExpression MINUSMINUS {
        $$ = build_operator1(OpDEC, $1);
    }
    ;

UnaryExpression 
    : PostfixExpression {}
    | Typeof UnaryExpression {
        $$ = build_typeof($2);
    }
    | PLUSPLUS   UnaryExpression {
        debug("++ UnaryExpression");
        $$ = build_operator1(OpINC, $2);
    }
    | MINUSMINUS UnaryExpression {
        debug("-- UnaryExpression");
        $$ = build_operator1(OpDEC, $2);
    }
    | ADD  UnaryExpression {
        debug("+ val");
        $$ = $2;
    }
    | SUB  UnaryExpression {
        debug("- val");
        $$ = build_operator1(OpMinus, $2);
    }
    | INV  UnaryExpression {
        debug("INV val");
        $$ = build_operator1(OpInv, $2);
    }
    | NOT  UnaryExpression {
        debug("NOT UnaryExpression");
        $$ = build_operator1(OpNot, $2);
    }
    ;

MultiplicativeExpression
    : UnaryExpression
    | MultiplicativeExpression MUL UnaryExpression {
        debug("MultiplicativeExpression1");
        $$ = build_operator2(OpMul, $1, $3);
    }
    | MultiplicativeExpression DIV UnaryExpression {
        debug0("MultiplicativeExpression2");
        $$ = build_operator2(OpDiv, $1, $3);
    }
    | MultiplicativeExpression REM UnaryExpression {
        debug0("MultiplicativeExpression3");
        $$ = build_operator2(OpMod, $1, $3);
    }
    ;

AdditiveExpression
    : MultiplicativeExpression
    | AdditiveExpression ADD MultiplicativeExpression {
        debug0("AdditiveExpression1");
        $$ = build_operator2(OpPlus, $1, $3);
    }
    | AdditiveExpression SUB MultiplicativeExpression {
        debug0("AdditiveExpression2");
        $$ = build_operator2(OpMinus, $1, $3);
    }
    ;

ShiftExpression 
    : AdditiveExpression
    | ShiftExpression LSHIFT AdditiveExpression {
        $$ = build_operator2(OpLSFTLET, $1, $3);
    }
    | ShiftExpression RSHIFT AdditiveExpression {
        $$ = build_operator2(OpRSFTLET, $1, $3);
    }
    ;

RelationalExpression 
    : ShiftExpression
    | RelationalExpression LT  ShiftExpression {
        $$ = build_operator2(OpLT, $1, $3);
    }
    | RelationalExpression GT  ShiftExpression {
        $$ = build_operator2(OpGT, $1, $3);
    }
    | RelationalExpression LTE ShiftExpression {
        $$ = build_operator2(OpLE, $1, $3);
    }
    | RelationalExpression GTE ShiftExpression {
        $$ = build_operator2(OpGE, $1, $3);
    }
    | RelationalExpression Instanceof ShiftExpression {
        $$ = build_instanceof($1, $3);
    }
    | RelationalExpression In ShiftExpression {
        $$ = build_operator2(OpIn, $1, $3);
    }
    ;

EqualityExpression 
    : RelationalExpression
    | EqualityExpression EQEQ RelationalExpression {
        $$ = build_operator2(OpEQ, $1, $3);
    }
    | EqualityExpression NEQ  RelationalExpression {
        $$ = build_operator2(OpNE, $1, $3);
    }
    ;

BitwiseANDExpression 
    : EqualityExpression
    | BitwiseANDExpression AND EqualityExpression {
        $$ = build_operator2(OpAnd, $1, $3);
    }
    ;

BitwiseXORExpression 
    : BitwiseANDExpression
    | BitwiseXORExpression XOR BitwiseANDExpression {
        $$ = build_operator2(OpXor, $1, $3);
    }
    ;

BitwiseORExpression 
    : BitwiseXORExpression
    | BitwiseORExpression OR BitwiseXORExpression {
        $$ = build_operator2(OpOr, $1, $3);
    }
    ;

LogicalANDExpression 
    : BitwiseORExpression
    | LogicalANDExpression LAND BitwiseORExpression
    ;

LogicalORExpression 
    : LogicalANDExpression
    | LogicalORExpression LOR LogicalANDExpression
    ;

