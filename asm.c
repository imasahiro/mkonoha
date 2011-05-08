#define T_DUMP(t) \
    fprintf(stderr, "%s:%d:", __FILE__, __LINE__);\
    type_is(O(t), TYPE_Token);\
    knh_dump(ctx, O(t))

static void asm_expr(Ctx *ctx, knh_Token_t *x);
static Reg_t asm_op2(Ctx *ctx, knh_Token_t *x);
static Reg_t register_alloc(struct vmcode_builder *cb, knh_Token_t *x0, int level);
static void asm_stmt_list(Ctx *ctx, knh_Token_t *stmt)
{
    int i;
    knh_Token_t *x;
    FOR_EACH_TOKEN(stmt, x, i) {
        asm_expr(ctx, x);
    }
}

#define CB (ctx->cb)
static void append_to_current_decls_tpl(Tuple(Token, Token) *tpl, int level)
{
    Array(Tuple) *a = g_ctx->blkdecls[level];
    if (!a) {
        a = Array_new(Tuple);
        g_ctx->blkdecls[level] = a;
    }
    Array_add(Tuple, a, cast(knh_Tuple_t*,tpl));
}
static void append_to_current_decls(knh_Token_t *name, knh_Token_t *val, int level)
{
    Tuple(Token, Token) *tpl = Tuple_new_init(Token, Token, name, val);
    append_to_current_decls_tpl(tpl, level);
}
static Array(Tuple) *get_current_level(int level)
{
    Array(Tuple) *a = g_ctx->blkdecls[level];
    if (!a) {
        a = Array_new(Tuple);
        g_ctx->blkdecls[level] = a;
    }
    return a;
}

struct asmdata;
typedef void (*fasm_p)(Ctx *, struct asmdata *, knh_Token_t *);
struct asmdata {
    union {
        Reg_t r;
        knh_value_t v;
    } v1, v2;
    fasm_p asm_p;
};

static knh_Token_t *get_variable(Ctx *ctx, knh_Token_t *name)
{
    Array(Tuple) *a = get_current_level(ctx->blklevel);
    knh_Tuple_t *x;
    Tuple(Token, Token) *tpl;
    int i;
    FOR_EACH_ARRAY(a, x, i) {
        tpl = cast(Tuple(Token, Token)*, x);
        if (string_cmp(tpl->o1->data.str, name->data.str) == 0) {
            return tpl->o2;
        }
    }
    return NULL;
}

static void asm_variable_name(Ctx *ctx, Reg_t r, knh_string_t *name, struct asmdata *dat)
{
    Array(Tuple) *a = get_current_level(ctx->blklevel);
    knh_Tuple_t *x;
    Tuple(Token, Token) *tpl;
    int i;
    FOR_EACH_ARRAY(a, x, i) {
        tpl = cast(Tuple(Token, Token)*, x);
        if (string_cmp(tpl->o1->data.str, name) == 0) {
            dat->asm_p(ctx, dat, tpl->o1);
            if (tpl->o1->type == TYPE_String) {
                CB->oset(CB, r, tpl->o2->data.o);
            } else if (tpl->o1->type == TYPE_Integer) {
                CB->nset_i(CB, r, tpl->o2->data.dval);
            } else if (tpl->o1->type == TYPE_Float) {
                CB->nset_f(CB, r, tpl->o2->data.fval);
            } else {
                if (Token_CODE(tpl->o1) == IDENTIFIER_NODE) {
                    knh_Token_t *o1 = get_variable(ctx, tpl->o1);
                    TODO();
                    fprintf(stderr, "%p\n", o1);
                }
                fprintf(stderr, "%p\n", tpl);
            }
        }
    }
}

static void asm_print_var_before(Ctx *ctx, struct asmdata *dat, knh_Token_t *x)
{
    CB->nset_i(CB, dat->v1.r, (x->type!=TYPE_UNTYPED)?x->type:TYPE_Integer);
}
static knh_string_t __equal = {1, "="};
static knh_string_t __camma = {2, ", "};
static knh_string_t __n     = {1, "\n"};
static void asm_call_print(Ctx *ctx, Array(Token) *a)
{
    int i;
    knh_Token_t *x;
    FOR_EACH_ARRAY_INIT(a, x, i, i=1) {
        CB->oset(CB, Arg0, O(ctx->out));
        if (i != 1) {
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(&__camma));
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        }
        if (Token_CODE(x) == IDENTIFIER_NODE) {
            struct asmdata dat = {};
            knh_string_t *name = x->data.str;
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(String_concat1(name, &__equal)));
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
            dat.v1.r = Arg1;
            dat.asm_p = asm_print_var_before;
            asm_variable_name(ctx, Arg2, name, &dat);
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == INTEGER_CONST) {
            CB->nset_i(CB, Arg1, TYPE_Integer);
            CB->nset_i(CB, Arg2, x->data.ival);
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == FLOAT_CONST) {
            CB->nset_i(CB, Arg1, TYPE_Float);
            CB->nset_f(CB, Arg2, x->data.fval);
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == STRING_CONST) {
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(x->data.str));
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        } else {
#define TokenType_IsNumber(x) (Token_type(x) == TYPE_Integer || Token_type(x) == TYPE_Float)
            Reg_t r = asm_op2(ctx, x);
            CB->nset_i(CB, Arg1, x->type);
            if (TokenType_IsNumber(x)) {
                CB->nmov(CB, Arg2, r);
            } else {
                CB->omov(CB, Arg2, r);
            }
            CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
        }
    }
    CB->nset_i(CB, Arg1, TYPE_String);
    CB->oset(CB, Arg2, O(&__n));
    CB->fcall(CB, Reg0, NULL, (void*)knh_OutputStream_print);
}
static knh_Method_t *findFunc_fromName(Ctx *ctx, knh_string_t *name)
{
    return NULL;
}
static void asm_call_expr(Ctx *ctx, knh_Token_t *stmt)
{
    int i;
    Array(Token) *a = cast(Array(Token)*, stmt->data.o);
    knh_Token_t *x, *func;
    T_DUMP(stmt);
    func = Array_n(a, 0);
    if (string_cmp_char(func->data.str, "print", 5) == 0) {
        asm_call_print(ctx, a);
    } else {
        knh_string_t *name = func->data.str;
        knh_Method_t *mtd = findFunc_fromName(ctx, name);
        FOR_EACH_ARRAY_INIT(a, x, i, i=1) {
            if (Token_CODE(x) == IDENTIFIER_NODE) {
                asm volatile("int3");
            } else if (Token_isConst(x)) {
                asm volatile("int3");
            }
        }
    }
}

static void asm_decl_tpl(Ctx *ctx, Tuple(Token, Token) *tpl)
{
    if (Token_isConst(tpl->o2)) {
        knh_value_t v;
        Reg_t r = register_alloc(CB, tpl->o1, 0);
        if (IS_ConstInt(tpl->o2)) {
            v.ival = tpl->o2->data.ival;
            CB->nset_i(CB, Reg1, v.dval);
        } else if (IS_ConstFloat(tpl->o2)) {
            v.fval = tpl->o2->data.fval;
            CB->nset_f(CB, Reg1, v.fval);
        } else {
            TODO();
        }
        tpl->o1->type = tpl->o2->type;
    } else {
        knh_Token_t *x0 = tpl->o1;
        knh_Token_t *x1 = tpl->o2;
        enum operator_code code = Token_CODE(x1);
        if (OpUndef < code && code < OPERATOR_CODE_MAX) {
            Reg_t r = asm_op2(ctx, x1);
        } else {
            TODO();
        }
    }
    append_to_current_decls_tpl(tpl, ctx->blklevel);
}

static void asm_decl(Ctx *ctx, knh_Token_t *stmt)
{
    Tuple(Token, Token) *tpl;
    T_DUMP(stmt);
    tpl = cast(Tuple(Token, Token)*, stmt->data.o);
    asm_decl_tpl(ctx, tpl);
}
DEF_TUPLE_T(Token, Reg);

DEF_TUPLE_S2_STRUCT(Token, Reg);
DEF_TUPLE_S2_OP(Token, Reg);

Array(Tuple) *g_regtable = NULL;
static Array(Tuple) *global_regtable(void)
{
    if (!g_regtable) {
        g_regtable = Array_new(Tuple);
    }
    return g_regtable;

}
static Reg_t register_alloc(struct vmcode_builder *cb, knh_Token_t *x0, int level)
{
    Array(Tuple) *regtable = global_regtable();
    knh_Tuple_t *x;Tuple(Token, Reg) *v;
    int i;
    FOR_EACH_ARRAY(regtable, x, i) {
        v = cast(Tuple(Token, Reg)*, x);
        if (v->o1 == x0) {
            return v->o2;
        }
    }
    {
        Reg_t r = cb->regalloc(cb, level);
        x = cast(knh_Tuple_t*, Tuple_new_init(Token, Reg, x0, r));
        Array_add(Tuple, regtable, x);
        return r;
    }
}

static Reg_t asm_0(Ctx *ctx, knh_Token_t *x0)
{
    Reg_t r = register_alloc(ctx->cb, x0, ctx->blklevel);
    if (Token_isConst(x0)) {
        CB->nset_i(CB, r, x0->data.ival);
    }
    else if (Token_CODE(x0) == IDENTIFIER_NODE) {
        //update_variable(ctx, x0, r);
    }
    return r;
}
static Reg_t asm_op2_const(Ctx *ctx, enum token_code op, knh_Token_t *x, knh_Token_t *x0, knh_Token_t *x1)
{
    Reg_t rtmp = Reg4;
    if (x0->type == TYPE_Integer) {
        if (x1->type == TYPE_Integer) {
            x->type = TYPE_Integer;
            knh_int_t val = x0->data.ival + x1->data.ival;
            CB->nset_i(CB, rtmp, val);
            return rtmp;
        } else if (x1->type == TYPE_Float) {
            x->type = TYPE_Float;
            knh_float_t val = (knh_float_t) x0->data.ival + x1->data.fval;
            CB->nset_f(CB, rtmp, val);
            return rtmp;
        }
    } else if (x0->type == TYPE_Float) {
        if (x1->type == TYPE_Integer) {
            x->type = TYPE_Float;
            knh_float_t val = x0->data.fval + (knh_float_t) x1->data.ival;
            CB->nset_f(CB, rtmp, val);
            return rtmp;
        } else if (x1->type == TYPE_Float) {
            x->type = TYPE_Float;
            knh_float_t val = x0->data.fval + x1->data.fval;
            CB->nset_f(CB, rtmp, val);
            return rtmp;
        }
    }
    return Reg0;
}
static Reg_t asm_op2_int_expr(Ctx *ctx, enum token_code code, Reg_t r1, Reg_t r2)
{
#define OPCASE(op) case Op##op:
    Reg_t r = CB->regalloc(CB, 0);
    switch (code) {
        OPCASE(Plus) {
            CB->iadd(CB, r, r1, r2);
            break;
        }
        default :
            TODO("int expr");
    }
    return r;
}
static Reg_t asm_op2_float_expr(Ctx *ctx, enum token_code code, Reg_t r0, Reg_t r1)
{
    Reg_t r = CB->regalloc(CB, 0);
    TODO("float expr");
    return r;
}
static Reg_t asm_op2_object_expr(Ctx *ctx, enum token_code code, Reg_t r0, Reg_t r1)
{
    Reg_t r = CB->regalloc(CB, 0);
    TODO("float expr");
    return r;
}

static Reg_t asm_op2(Ctx *ctx, knh_Token_t *x)
{
    Array(Token) *tk = cast(Array(Token)*, x->data.o);
    T_DUMP(x);
    Tuple(Token, Token) *tpl;
    knh_Token_t *x0, *x1;
    enum token_code code = Token_CODE(x);
    x0 = Array_n(tk, 0);
    x1 = Array_n(tk, 1);
#define OPCASE(op) case Op##op:
    switch (code) {
        OPCASE(EQLET) {
            tpl = Tuple_new_init(Token, Token, x0, x1);
            asm_decl_tpl(ctx, tpl);
            break;
        }
        OPCASE(Plus) {
            Reg_t r0, r1;
            if (Token_isConst(x0) && Token_isConst(x1)) {
                return asm_op2_const(ctx, code, x, x0, x1);
            }
            r0 = asm_0(ctx, x0);
            r1 = asm_0(ctx, x1);
            if (x0->type == TYPE_Integer) {
                Reg_t rtmp = Reg4;
                if (x1->type == TYPE_Integer) {
                    x->type = TYPE_Integer;
                    knh_int_t val = x0->data.ival + x1->data.ival;
                    CB->nset_i(CB, rtmp, val);
                    return rtmp;
                } else {
                    Reg_t r1 = asm_0(ctx, x1);
                    CB->nset_i(CB, rtmp, x0->data.ival);
                    CB->iadd(CB, rtmp, r1, rtmp);
                    TODO();
                }
            } else {
                if (x1->type == TYPE_Integer) {
                    x0->type = TYPE_Integer;
                    x->type  = TYPE_Integer;
                    return asm_op2_int_expr(ctx, code, r0, r1);
                }
                else if (x1->type == TYPE_Integer) {
                    TODO("Float typing");
                    return asm_op2_float_expr(ctx, code, r0, r1);
                } else {
                    TODO("Object typing");
                    return asm_op2_object_expr(ctx, code, r0, r1);
                }
                asm volatile("int3");
                fprintf(stderr, "\n");
            }
        }
        default:
            asm volatile("int3");
            break;
    }
#undef OPCASE
    return Reg0;
}

static void asm_expr(Ctx *ctx, knh_Token_t *x)
{
    enum token_code code = Token_CODE(x);
    switch(code) {
        case STMT_LIST:
            asm_stmt_list(ctx, x);
            break;
        case CALL_EXPR:
            asm_call_expr(ctx, x);
            break;
        case VAR_DECL:
            asm_decl(ctx, x);
            break;
        default:
            if (OpUndef < code && code < OPERATOR_CODE_MAX) {
                asm_op2(ctx, x);
                break;
            }
            asm volatile("int3");
            konoha_error("???");
    }
}

void write_global_script(knh_Token_t *stmt)
{
    int i = 0;
    knh_Token_t *x;
    Ctx *ctx = g_ctx;
    struct vmcode_builder *cb = new_vmcode_builder(ctx->vm);
    ctx->cb = cb;
    token_check(stmt, code_is, STMT_LIST);
    FOR_EACH_TOKEN(stmt, x, i) {
        asm_expr(ctx, x);
    }
}

