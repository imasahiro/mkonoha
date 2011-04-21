#ifndef KONOHA_H
#define KONOHA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define IO_DEBUG 1

#define cast(T, p) ((T) p)
#define KNH_T(T) knh_##T##_t

struct knh_Object_t;
typedef struct knh_Object_t knh_Object_t;

struct knh_Token_t;
typedef struct knh_Token_t knh_Token_t;

struct context;
typedef struct context Ctx;
typedef const Ctx *const CTX;

typedef long long int knh_int_t;
typedef double        knh_float_t;

int konoha_lex(CTX ctx);
int konoha_error (const char *);
void dbg_p(const char *file, const char *func, int line, const char *fmt, ...);
void knh_dump(CTX ctx, knh_Object_t *o);

#include "object.h"
#include "array.h"
#include "tuple.h"
#include "token.h"

typedef enum operator_code KOperator;

/* types decl */
//##define @Generics Array<Token>
DEF_ARRAY_T(Token);
DEF_TUPLE_T(Token, Token);

DEF_ARRAY_S_T(int);
DEF_ARRAY_T(Object);

knh_Token_t *konoha_build_id(const char *);
knh_Token_t *konoha_build_int(int);
knh_Token_t *konoha_build_float(float val);
knh_Token_t *konoha_build_string(knh_string_t *val);
void knh_dump(CTX ctx, knh_Object_t *o);


knh_Token_t *variable_decl(CTX ctx, knh_Token_t *type, Array(Token) *vec);
knh_Token_t *knh_Token_toType(knh_Token_t *t);
knh_Token_t *build_assignment(knh_Token_t *expr);
knh_Token_t *build_variable_decl(knh_Token_t *name, knh_Token_t *init);
knh_Token_t *build_assignment_expr(KOperator op, knh_Token_t *t1, knh_Token_t *t2);
knh_Token_t *build_throw_expr(knh_Token_t *t);
knh_Token_t *build_operator2(KOperator op, knh_Token_t *t1, knh_Token_t *t2);
knh_Token_t *build_operator1(KOperator op, knh_Token_t *t1);
knh_Token_t *build_typeof(knh_Token_t *t1);
knh_Token_t *build_instanceof(knh_Token_t *t1, knh_Token_t *t2);
knh_Token_t *build_reutrn(knh_Token_t *expr);
knh_Token_t *build_try_catch(knh_Token_t *t, knh_Token_t *c, knh_Token_t *f);
knh_Token_t *build_catch(knh_Token_t *name, knh_Token_t *block);
knh_Token_t *build_final(knh_Token_t *block);
knh_Token_t *build_for(knh_Token_t *valist, knh_Token_t *expr1, knh_Token_t *expr2, knh_Token_t *body);
knh_Token_t *build_for_decl(knh_Token_t *type, Array(Token) *valist, knh_Token_t *expr1, knh_Token_t *expr2, knh_Token_t *body);
knh_Token_t *build_while(knh_Token_t *expr, knh_Token_t *body);
knh_Token_t *build_if_else(knh_Token_t *cond, knh_Token_t *b1, knh_Token_t *b2);
knh_Token_t *build_foreach(knh_Token_t *type, knh_Token_t *var, knh_Token_t *itr, knh_Token_t *stmt);

knh_Token_t *new_TokenStmtList(void);
void knh_TokenStmtList_add(knh_Token_t *stmts, knh_Token_t *t);
void write_global_script(knh_Token_t *stmt);
knh_Token_t *build_function_decl(knh_Token_t *type, knh_Token_t *name, Array(Token) *param, knh_Token_t *body);
knh_Token_t *build_typed_variable(knh_Token_t *type, knh_Token_t *val);
knh_Token_t *build_call_expr(knh_Token_t *func, Array(Token) *args);

#endif /* end of include guard: KONOHA_H */
