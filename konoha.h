#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#ifndef KONOHA_H
#define KONOHA_H

#define cast(T, p) ((T) p)
#define KNH_T(T) knh_##T##_t

struct knh_Object_t;
typedef struct knh_Object_t knh_Object_t;

struct knh_Token_t;
typedef struct knh_Token_t knh_Token_t;

enum KONOHA_OPERATOR {
    KONOHA_ADD, 
};

typedef enum KONOHA_OPERATOR KOperator;

struct context;
typedef struct context Ctx;
typedef const Ctx *const CTX;

int konoha_lex(CTX ctx);
int konoha_error (const char *);
void dbg_p(const char *file, const char *func, int line, const char *fmt, ...);
void knh_dump(CTX ctx, knh_Object_t *o);

#include "object.h"
#include "array.h"
#include "tuple.h"
#include "token.h"

/* types decl */
//##define @Generics Array<Token>
DEF_ARRAY_T(Token);
DEF_TUPLE_T(Token, Token);


knh_Token_t *variable_decl(CTX ctx, knh_Token_t *type, Array(Token) *vec);
knh_Token_t *knh_Token_toType(knh_Token_t *t);
knh_Token_t *build_assignment(knh_Token_t *expr);
knh_Token_t *build_variable_decl(knh_Token_t *name, knh_Token_t *init);
knh_Token_t *build_assignment_expr(KOperator op, knh_Token_t *t1, knh_Token_t *t2);

#endif /* end of include guard: KONOHA_H */
