#include "konoha.h"
#include "memory.h"
#include "stream.h"
#include "vm.h"
#include "kstring.c"
#ifndef TEST
#define KONOHA
#endif

#define TYPE_(cid)       ((knh_class_t)(cid))
#define TYPEINFO_(ctx, cid) ((ctx)->types+(cid))

#define string_copy(s) (copy_(knh_string_t*, s, sizeof(knh_string_t) + s->len))

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))
#define FOR_EACH_STATIC(a, x, i) for(i=0, x = a; i < _ARRAY_SIZE(a); x=&a[(++i)])
#define TODO(msg) {\
    konoha_error("TODO:" msg);\
    asm volatile("int3");\
}


#define FOR_EACH_TOKEN(stt, x, i) \
    FOR_EACH_ARRAY(((Array(Token)*)stt->data.o), x, i)

DEF_TUPLE_STRUCT(Token, Token);
DEF_TUPLE_OP(Token, Token);
DEF_ARRAY_STRUCT(Token);
DEF_ARRAY_OP(Token);

DEF_ARRAY_T(Tuple);
DEF_ARRAY_STRUCT(Tuple);
DEF_ARRAY_OP(Tuple);

DEF_ARRAY_S_T(class);
DEF_ARRAY_S_STRUCT(class);
DEF_ARRAY_S_OP(class);

struct type_info {
    const char *name;
    knh_class_t cid, bcid;
    knh_class_t suppercid;
    Array(class) *param;
    void (*write_) (CTX ctx, struct io *io, knh_Object_t *o);
};

struct context {
    struct type_info *types;
    int type_size;
    struct alias_info *alias;
    int alias_size;
    Array(Tuple) *decl_list;
    struct vm *vm;
    struct io *_out;
    struct io *_err;
    struct io *_in;
    struct knh_OutputStream_t *out;
    struct knh_OutputStream_t *err;
    struct knh_InputStream_t  *in;

    struct vmcode_builder *cb;
    Array(Tuple) **blkdecls;
    int blklevel;
};

struct alias_info {
    const char *n1;
    const char *n2;
};

struct knh_Token_t {
    knh_hObject_t h;
    enum token_code     code;
    enum operator_code  op;
    knh_class_t         type;
    knh_value_t         data;
};

static Ctx *new_context(void);
static void context_delete(CTX ctx);
static knh_class_t append_new_class(knh_class_t bcid, Array(class) *param);
extern FILE *konoha_in;
extern int konoha_parse(CTX ctx);
static struct context *g_ctx = NULL;

#ifdef KONOHA
int main(int argc, char **argv)
{
    if (argc >= 2) {
        CTX ctx = new_context();
        struct vm *vm;
        g_ctx = cast(struct context*, ctx);
        konoha_in = fopen(argv[1], "r");
        konoha_parse(ctx);
        vm = ctx->vm;
        vm_exec(vm, ctx->cb->emit_code(ctx->cb));
        fclose(konoha_in);
        context_delete(ctx);
        return 0;
    } else {
        fprintf(stderr, "%s filename ...\n", argv[0]);
        return 0;
    }
}
#endif

static inline void *__new(size_t size, knh_class_t cid)
{
    knh_Object_t *o = malloc_(size);
    o->h.magicflag  = MINIKONOHA_MAGIC;
    o->h.classinfo  = TYPE_(cid);
    return o;
}

static inline void *__copy(void *v, size_t size)
{
    void *p = malloc(size);
    memcpy(p, v, size);
    return p;
}

static inline knh_string_t *new_string_(const char *txt)
{
    int len = strlen(txt) + 1;
    knh_string_t *s = malloc_(sizeof(knh_string_t) + len);
    s->len = len;
    memcpy(s->txt, txt, len);
    return s;
}

knh_string_t *new_string(const char *txt)
{
    return new_string_(txt);
}

void dbg_p(const char *file, const char *func, int line, const char *fmt, ...)
{
    va_list ap;
    va_start(ap , fmt);
    fflush(stdout);
    fprintf(stderr, "DEBUG(%s:%d) ", func, line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

void knh_dump(CTX ctx, knh_Object_t *o)
{
    struct type_info *typeinfo = ctx->types + o->h.classinfo;
    typeinfo->write_(ctx, ctx->_err, o);
    io_puts(ctx->_err, "\n", 1);
}

#include "ctx.c"

knh_Array_t *new_Array(void)
{
    knh_Array_t *a = new_(Array);
    a->type = TYPE_UNTYPED;
    a->size = 0;
    a->capacity = 1;
    a->list = malloc_(sizeof(void*));
    return a;
}
knh_Tuple_t *new_Tuple(void)
{
    knh_Tuple_t *t = new_(Tuple);
    t->type1 = TYPE_UNTYPED;
    t->type2 = TYPE_UNTYPED;
    t->o1 = t->o2 = NULL;
    return t;
}
static const char *convert_alias(const char *s)
{
    int i;
    Ctx *ctx = g_ctx;
    struct alias_info *x;
    for (i = 0; i < ctx->alias_size; i++) {
        x = ctx->alias + i;
        if (strcmp(x->n1, s) == 0)
            return x->n2;
    }
    return s;
}
knh_Token_t *konoha_build_id(const char *name)
{
    knh_Token_t *t = new_(Token);
    t->code = IDENTIFIER_NODE;
    t->type = TYPE_UNTYPED;
    t->data.str = new_string_(convert_alias(name));
    return t;
}
knh_Token_t *konoha_build_int(int val)
{
    knh_Token_t *t = new_(Token);
    t->code = INTEGER_CONST;
    t->type = TYPE_Integer;
    t->data.ival = val;
    return t;
}
knh_Token_t *konoha_build_float(float val)
{
    knh_Token_t *t = new_(Token);
    t->code = FLOAT_CONST;
    t->type = TYPE_Float;
    t->data.fval = val;
    return t;
}
knh_Token_t *konoha_build_string(knh_string_t *val)
{
    knh_Token_t *t = new_(Token);
    t->code = STRING_CONST;
    t->type = TYPE_String;
    t->data.str = val;
    return t;
}

#define token_check(t, op, v) (assert(token_##op(t, v)))
#define type_assert(o, op, t) (assert(type_##op(cast(knh_Object_t*,o), TYPE_##t)))
#define type_cast(T, o) ((knh_##T##_t*)(o))

static inline int type_is(knh_Object_t *o, knh_class_t cid)
{
    return o->h.classinfo == cid;
}

static inline int token_code_is(knh_Token_t *t, enum token_code code)
{
    return t->code == code;
}
static knh_class_t string_to_type(knh_string_t *s)
{
    int i;
    Ctx *ctx = g_ctx;
    struct type_info *x;
    for (i = 0; i < ctx->type_size; i++) {
        x = ctx->types + i;
        if (strcmp(x->name, s->txt) == 0) {
            return x->cid;
        }
    }
    return TYPE_UNTYPED;
}

knh_Token_t *knh_Token_toType(knh_Token_t *t)
{
    type_assert(t, is, Token);
    token_check(t, code_is, IDENTIFIER_NODE);
    t->type = string_to_type(t->data.str);
    t->code = TYPE_NODE;
    return t;
}

static inline knh_Token_t *token_op(knh_Token_t *t, enum token_code code)
{
    assert(t->code == code);
    return t;
}
#define TOKEN_TYPE(t)  ((t)->type)
#define Token_tocid(t) (TOKEN_TYPE(token_op(t, TYPE_NODE)))

knh_Token_t *new_TokenStmtList(void)
{
    knh_Token_t *t = new_(Token);
    t->code   = STMT_LIST;
    t->type   = TYPE_UNTYPED;
    t->data.o = O(Array_new(Token));
    return t;
}

static knh_Token_t *new_TokenStmtList__(Array(Token) *a)
{
    knh_Token_t *t = new_TokenStmtList();
    Array_delete(Token, (Array(Token) *)t->data.o);
    t->data.o = O(a);
    return t;
}

void knh_TokenStmtList_add(knh_Token_t *stmts, knh_Token_t *t)
{
    Array(Token) *a = (Array(Token) *) stmts->data.o;
    Array_add(Token, a, t);
}

static void Token_typing(knh_Token_t *t, knh_class_t cid)
{
    if (TOKEN_TYPE(t) == TYPE_UNTYPED) {
        TOKEN_TYPE(t) = cid;
    } else {
        asm volatile("int3");
    }
    /* TODO */
    /* traverse token and convert each tokens */
}

knh_Token_t *variable_decl(CTX ctx, knh_Token_t *type, Array(Token) *vec)
{
    if (type) {
        knh_class_t cid = Token_tocid(type);
        int i;
        knh_Token_t *x;
        FOR_EACH_ARRAY(vec, x, i) {
            Token_typing(x, cid);
        }
    } else {
        /* decl untyped variable */
        asm volatile("int3");
    }
    return new_TokenStmtList__(vec);
}

knh_Token_t *build_variable_decl(knh_Token_t *name, knh_Token_t *init)
{
    knh_Token_t *t = new_(Token);
    Tuple(Token, Token) *tpl = Tuple_new_init(Token, Token, name, init);
    t->code   = VAR_DECL;
    t->type   = TYPE_UNTYPED;
    t->data.o = O(tpl);
    push_decl(g_ctx, tpl);
    return t;
}

knh_Token_t *build_assignment(knh_Token_t *expr)
{
    TODO();
    return NULL;
}

static knh_class_t typing2(knh_class_t dst, knh_class_t src)
{
    /* TODO typing token "t" */
    return TYPE_UNTYPED;
}

static knh_class_t typing1(knh_class_t dst)
{
    /* TODO typing token "t" */
    return TYPE_UNTYPED;
}

#define IS_ConstInt(o) (Token_CODE(o) == INTEGER_CONST)
#define IS_ConstFloat(o) (Token_CODE(o) == FLOAT_CONST)
knh_Token_t *build_assignment_expr(KOperator op, knh_Token_t *t1, knh_Token_t *t2)
{
    knh_Token_t *t = new_(Token);
    Array(Token) *a = Array_new(Token);
    Array_add(Token, a, t1);
    Array_add(Token, a, t2);
    t->code   = op;
    t->type   = typing2(t1->type, t2->type);
    t->data.o = O(a);
    return t;
}
knh_Token_t *build_throw_expr(knh_Token_t *t)
{
    TODO();
    return NULL;
}
knh_Token_t *build_operator2(KOperator op, knh_Token_t *t1, knh_Token_t *t2)
{
    knh_Token_t *t = new_(Token);
    Array(Token) *a = Array_new(Token);
    Array_add(Token, a, t1);
    Array_add(Token, a, t2);
    t->code   = op;
    t->type   = typing2(t1->type, t2->type);
    t->data.o = O(a);
    return t;
}
knh_Token_t *build_operator1(KOperator op, knh_Token_t *t1)
{
    knh_Token_t *t = new_(Token);
    Array(Token) *a = Array_new(Token);
    Array_add(Token, a, t1);
    t->code   = op;
    t->type   = typing1(t1->type);
    t->data.o = O(a);
    return t;
}
knh_Token_t *build_typeof(knh_Token_t *t1)
{
    TODO();
    return NULL;
}
knh_Token_t *build_instanceof(knh_Token_t *t1, knh_Token_t *t2)
{
    TODO();
    return NULL;
}
knh_Token_t *build_reutrn(knh_Token_t *expr)
{
    knh_Token_t *t = new_(Token);
    t->code   = RETUEN_NODE;
    t->type   = TYPE_UNTYPED;
    t->data.o = O(expr);
    return t;

}
knh_Token_t *build_try_catch(knh_Token_t *t, knh_Token_t *c, knh_Token_t *f)
{
    TODO();
    return NULL;
}
knh_Token_t *build_catch(knh_Token_t *name, knh_Token_t *block)
{
    TODO();
    return NULL;
}
knh_Token_t *build_final(knh_Token_t *block)
{
    TODO();
    return NULL;
}
knh_Token_t *build_for(knh_Token_t *expr1, knh_Token_t *expr2, knh_Token_t *expr3, knh_Token_t *body)
{
    TODO();
    return NULL;
}
knh_Token_t *build_for_decl(knh_Token_t *type, Array(Token) *valist, knh_Token_t *expr1, knh_Token_t *expr2, knh_Token_t *body)
{
    TODO();
    return NULL;
}
knh_Token_t *build_while(knh_Token_t *expr, knh_Token_t *body)
{
    TODO();
    return NULL;
}
knh_Token_t *build_if_else(knh_Token_t *cond, knh_Token_t *b1, knh_Token_t *b2)
{
    TODO();
    return NULL;
}
knh_Token_t *build_foreach(knh_Token_t *type, knh_Token_t *var, knh_Token_t *itr, knh_Token_t *stmt)
{
    TODO();
    return NULL;
}

int konoha_error (const char *msg)
{
  fprintf(stderr, "%s", msg);
  exit(1);
}

#include "asm.c"

static knh_class_t build_function_type(knh_Token_t *rtype, Array(Token) *param);

knh_Token_t *build_function_decl(knh_Token_t *type, knh_Token_t *name, Array(Token) *param, knh_Token_t *body)
{
    //fprintf(stderr, "type\n");
    //knh_dump(g_ctx, O(type));
    //fprintf(stderr, "name\n");
    //knh_dump(g_ctx, O(name));
    //fprintf(stderr, "param\n");
    //knh_dump(g_ctx, O(param));
    //fprintf(stderr, "type\n");
    //knh_dump(g_ctx, O(body));

    int i;
    knh_Token_t *x, *t = new_(Token);
    knh_class_t fn_type = build_function_type(type, param);
    Array(Token) *a = Array_new(Token);
    Array_add(Token, a, name);
    Array_add(Token, a, body);

    FOR_EACH_ARRAY(param, x, i) {
        Array_add(Token, a, x);
    }
    t->code   = FUNCTION_DECL;
    t->type   = fn_type;
    t->data.o = O(a);
    return t;
}

static knh_class_t build_function_type(knh_Token_t *rtype, Array(Token) *param)
{
    int i;
    knh_Token_t *x;
    knh_class_t rettype = (rtype)? rtype->type: TYPE_UNTYPED;
    Array(class) *a = Array_new(class);
    Array_add(class, a, rettype);
    FOR_EACH_ARRAY(param, x, i) {
        Array_add(class, a, x->type);
    }
    return append_new_class(TYPE_Method, a);
}

knh_Token_t *build_typed_variable(knh_Token_t *type, knh_Token_t *val)
{
    val->type = string_to_type(type->data.str);
    return val;
}

knh_Token_t *build_call_expr(knh_Token_t *func, Array(Token) *args)
{
    int i;
    knh_Token_t *x, *t = new_(Token);
    Array(Token) *a = Array_new(Token);
    Array_add(Token, a, func);
    FOR_EACH_ARRAY(args, x, i) {
        Array_add(Token, a, x);
    }
    t->code   = CALL_EXPR;
    t->type   = TYPE_UNTYPED;
    t->data.o = O(a);
    return t;
}

#include "stream.c"
#define DBG_VM
#include "vm.c"
