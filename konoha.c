#include "konoha.h"

#define TYPE_(cid)       ((knh_class_t)(cid))

#define malloc_(len) (malloc(len))
#define new_(T)      (knh_##T##_t *) __new(sizeof(knh_##T##_t), TYPE_##T)
#define delete_(ptr) { \
    free(ptr); \
    ptr = NULL;\
}
#define copy_(T, v, size) ((T) __copy(v, size))
#define copy_1(T, v) copy_(T, v, sizeof(T))
#define string_copy(s) (copy_(knh_string_t*, s, sizeof(knh_string_t) + s->len))

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))
#define FOR_EACH_STATIC(a, x, i) for(i=0, x = a; i < _ARRAY_SIZE(a); x=&a[(++i)])

DEF_TUPLE_STRUCT(Token, Token);
DEF_TUPLE_OP(Token, Token);
DEF_ARRAY_STRUCT(Token);
DEF_ARRAY_OP(Token);

DEF_ARRAY_T(Tuple);
DEF_ARRAY_STRUCT(Tuple);
DEF_ARRAY_OP(Tuple);

struct type_info {
    const char *name;
    knh_class_t cid, bcid;
    knh_class_t suppercid;
    void (*write_) (CTX ctx, FILE *fd, knh_Object_t *o);
};

struct context {
    struct type_info *types;
    int type_size;
    struct alias_info *alias;
    int alias_size;
    Array(Tuple) *decl_list;
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

static CTX new_context(void);
extern FILE *konoha_in;
extern int konoha_parse(CTX ctx);
static struct context *g_ctx = NULL;

int main(int argc, char **argv)
{
    if (argc >= 2) {
        CTX ctx = new_context();
        g_ctx = cast(struct context*, ctx);
        konoha_in = fopen(argv[1], "r");
        konoha_parse(ctx);
        fclose(konoha_in);
        return 0;
    } else {
        fprintf(stderr, "%s filename ...\n", argv[0]);
        return 0;
    }
}

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

static inline knh_string_t *new_string(const char *txt)
{
    int len = strlen(txt) + 1;
    knh_string_t *s = malloc_(sizeof(knh_string_t) + len);
    s->len = len;
    memcpy(s->txt, txt, len);
    return s;
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
    typeinfo->write_(ctx, stderr, o);
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
    t->data.str = new_string(convert_alias(name));
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
    return NULL;
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

#define FOR_EACH_ARRAY(a, x, i) \
    for(i=0, x = a->list[0]; i < (a)->size; x=a->list[(++i)])

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
    return vec;
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
    return NULL;
}

knh_Token_t *build_assignment_expr(KOperator op, knh_Token_t *t1, knh_Token_t *t2)
{
    return NULL;
}

knh_Token_t *new_TokenStmtList(void)
{
    knh_Token_t *t = new_(Token);
    t->code   = STMT_LIST;
    t->type   = TYPE_UNTYPED;
    t->data.o = NULL;
    return t;
}

void knh_TokenStmtList_add(knh_Token_t *stmts, knh_Token_t *t)
{
    Array(Token) *a;
    if (!t->data.o) {
        t->data.o = O(Array_new(Token));
    }
    a = (Array(Token) *) t->data.o;
    Array_add(Token, a, t);
}

