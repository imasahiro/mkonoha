#define T_DUMP(t) \
    fprintf(stderr, "%s:%d:", __FILE__, __LINE__);\
    type_is(O(t), TYPE_Token);\
    knh_dump(ctx, O(t))

static void asm_expr(Ctx *ctx, knh_Token_t *x);
static void asm_call_expr(Ctx *ctx, knh_Token_t *stmt);
static Reg_t asm_op2(Ctx *ctx, knh_Token_t *x);
static Reg_t register_alloc(struct vmcode_builder *cb, knh_Token_t *x0, int level);
static void register_clear(struct vmcode_builder *cb, int level);
static Reg_t register_alloc_arg(struct vmcode_builder *cb, int i);
static void register_update(Ctx *ctx, Reg_t reg, knh_Token_t *x);
static knh_Method_t *findFunc_fromName(Ctx *ctx, knh_string_t *name);
static void asm_stmt_list(Ctx *ctx, knh_Token_t *stmt)
{
    int i;
    knh_Token_t *x;
    FOR_EACH_TOKEN(stmt, x, i) {
        asm_expr(ctx, x);
    }
}

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
                    Reg_t r0 = register_alloc(CB, tpl->o2, ctx->blklevel);
                    CB->nmov(CB, r, r0);
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
static knh_class_t Method_getRetType(knh_Method_t *mtd)
{
    Ctx *ctx = g_ctx;
    struct type_info *typeinfo = ctx->types + mtd->cid;
    if (typeinfo->param) {
        return Array_n(typeinfo->param,0);
    }
    return TYPE_UNTYPED;
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
#define RET_REG Arg6
        if (i != 1) {
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(&__camma));
            /* TODO fcal without retval*/
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
        if (Token_CODE(x) == IDENTIFIER_NODE) {
            struct asmdata dat = {};
            knh_string_t *name = x->data.str;
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(String_concat1(name, &__equal)));
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
            dat.v1.r = Arg1;
            dat.asm_p = asm_print_var_before;
            asm_variable_name(ctx, Arg2, name, &dat);
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == INTEGER_CONST) {
            CB->nset_i(CB, Arg1, TYPE_Integer);
            CB->nset_i(CB, Arg2, x->data.ival);
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == FLOAT_CONST) {
            CB->nset_i(CB, Arg1, TYPE_Float);
            CB->nset_f(CB, Arg2, x->data.fval);
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == STRING_CONST) {
            CB->nset_i(CB, Arg1, TYPE_String);
            CB->oset(CB, Arg2, O(x->data.str));
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
        else if (Token_CODE(x) == CALL_EXPR) {
            Reg_t r;
            knh_string_t *name = Array_n(cast(Array(Token)*,x->data.o),0)->data.str;
            knh_Method_t *mtd = findFunc_fromName(ctx, name);
            knh_class_t rcid = Method_getRetType(mtd);
            asm_call_expr(ctx, x);
            r = register_alloc(CB, x, ctx->blklevel);
            CB->oset(CB, Arg0, O(ctx->out));
            CB->nset_i(CB, Arg1, rcid);
            if (rcid == TYPE_Integer || rcid == TYPE_Float) {
                CB->nmov(CB, Arg2, r);
            } else {
                CB->omov(CB, Arg2, r);
            }
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        } else {
#define TokenType_IsNumber(x) (Token_type(x) == TYPE_Integer || Token_type(x) == TYPE_Float)
            Reg_t r = asm_op2(ctx, x);
            CB->nset_i(CB, Arg1, x->type);
            if (TokenType_IsNumber(x)) {
                CB->nmov(CB, Arg2, r);
            } else {
                CB->omov(CB, Arg2, r);
            }
            CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
        }
    }
    CB->nset_i(CB, Arg1, TYPE_String);
    CB->oset(CB, Arg2, O(&__n));
    CB->fcall(CB, RET_REG, NULL, (void*)knh_OutputStream_print);
}
static knh_Method_t *findFunc_fromName(Ctx *ctx, knh_string_t *name)
{
    int i;
    Array(Method) *mtable = ctx->mtable;
    if (mtable) {
        knh_Method_t *mtd;
        FOR_EACH_ARRAY(mtable, mtd, i) {
            if (string_cmp(mtd->name, name) == 0) {
                return mtd;
            }
        }
    }
    TODO("function not found");
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
        Reg_t r0;
        FOR_EACH_ARRAY_INIT(a, x, i, i=1) {
            Reg_t r = register_alloc_arg(CB, i-1);
            if (Token_CODE(x) == IDENTIFIER_NODE) {
                asm volatile("int3");
                TODO("call arg id");
            } else if (Token_isConst(x)) {
                CB->nset_i(CB, r, x->data.ival);
            } else  {
                asm volatile("int3");
                TODO("call arg expr");
            }
        }
        r0 = register_alloc(CB, stmt, 0);
        CB->call(CB, r0, mtd, NULL);
    }
}

static void asm_decl_tpl(Ctx *ctx, Tuple(Token, Token) *tpl)
{
    if (Token_isConst(tpl->o2)) {
        knh_value_t v;
        Reg_t r0 = register_alloc(CB, tpl->o1, 0);
        if (IS_ConstInt(tpl->o2)) {
            v.ival = tpl->o2->data.ival;
            CB->nset_i(CB, r0, v.dval);
        } else if (IS_ConstFloat(tpl->o2)) {
            v.fval = tpl->o2->data.fval;
            CB->nset_f(CB, r0, v.fval);
        } else {
            TODO();
        }
        tpl->o1->type = tpl->o2->type;
    } else {
        knh_Token_t *x0 = tpl->o1;
        knh_Token_t *x1 = tpl->o2;
        enum operator_code code = Token_CODE(x1);
        if (OpUndef < code && code < OPERATOR_CODE_MAX) {
            asm_op2(ctx, x1);
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
static Reg_t register_alloc_arg(struct vmcode_builder *cb, int i)
{
    if (i > Arg11) {
        TODO("Too Many args");
    }
    return (Reg_t)Arg0 + i;
}
Array(Token) *g_args;
static void push_args(Ctx *ctx, Array(Token) *x0)
{
    g_args = x0;
}

static Reg_t register_alloc(struct vmcode_builder *cb, knh_Token_t *x0, int level)
{
    Array(Tuple) *regtable = global_regtable();
    knh_Tuple_t *x;Tuple(Token, Reg) *v;
    int i;
    if (Token_CODE(x0) == IDENTIFIER_NODE) {
        knh_Token_t *y;
        FOR_EACH_ARRAY_INIT(g_args, y, i, i=2) {
            if (string_cmp(y->data.str, x0->data.str) == 0) {
                return register_alloc_arg(cb, i-2);;
            }
        }
    }
    FOR_EACH_ARRAY(regtable, x, i) {
        v = cast(Tuple(Token, Reg)*, x);
        if (!v->o1 || v->o2 == -1) {
            continue;
        }
        if (v->o1 == x0) {
            return v->o2;
        }
        if (type_is(O(v->o1), TYPE_Token)) {
            if (Token_CODE(v->o1) == IDENTIFIER_NODE && Token_CODE(x0) == IDENTIFIER_NODE) {
                if (string_cmp(v->o1->data.str, x0->data.str) == 0) {
                    return v->o2;
                }
            }
        }
    }
    {
        Reg_t r = cb->regalloc(cb, level);
        x = cast(knh_Tuple_t*, Tuple_new_init(Token, Reg, x0, r));
        Array_add(Tuple, regtable, x);
        return r;
    }
}
static void register_clear(struct vmcode_builder *cb, int level)
{
    Array(Tuple) *regtable = global_regtable();
    knh_Tuple_t *x;Tuple(Token, Reg) *v;
    int i;
    FOR_EACH_ARRAY(regtable, x, i) {
        Tuple(Token, Reg) *v = cast(Tuple(Token, Reg)*, x);
        v->o1 = NULL;
        v->o2 = -1;
    }
}
static void register_update(Ctx *ctx, Reg_t reg, knh_Token_t *x0)
{
    Array(Tuple) *regtable = global_regtable();
    knh_Tuple_t *x;Tuple(Token, Reg) *v;
    int i;
    FOR_EACH_ARRAY(regtable, x, i) {
        v = cast(Tuple(Token, Reg)*, x);
        if (v->o1 == x0) {
            TODO("???");
            asm volatile("int3");
        }
        if (v->o2 == reg) {
            asm volatile("int3");
            TODO("same register????");
        }
    }
    x = cast(knh_Tuple_t*, Tuple_new_init(Token, Reg, x0, reg));
    Array_add(Tuple, regtable, x);
}

static Reg_t asm_0(Ctx *ctx, knh_Token_t *x0)
{
    Reg_t r = register_alloc(ctx->cb, x0, ctx->blklevel);
    if (Token_isConst(x0)) {
        CB->nset_i(CB, r, x0->data.ival);
    }
    else if (Token_CODE(x0) == IDENTIFIER_NODE) {
        fprintf(stderr, "reg=%d, x0=%p\n", r, x0);
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
                    Reg_t reg = asm_op2_int_expr(ctx, code, r0, r1);
                    register_update(ctx, reg, x);
                    return reg;
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
    TODO("not match");
    return Reg0;
}
static knh_Method_t *add_newfunction(Ctx *ctx, knh_class_t cid, knh_string_t *name, fvm2 func)
{
    knh_Method_t *mtd = new_Method(func, NULL);
    mtd->cid = cid;
    mtd->name = name;
    Array_add(Method, ctx->mtable, mtd);
    return mtd;
}
static void function_decl(Ctx *ctx, knh_Token_t *x0)
{
    Array(Token) *a;
    knh_Token_t *x;
    knh_Method_t *mtd;
    struct vmcode_builder *oldcb, *cb = new_vmcode_builder(ctx->vm);
    oldcb = ctx->cb; ctx->cb = cb;

    a = cast(Array(Token)*, x0->data.o);
    x = Array_n(a, 0);
    mtd = add_newfunction(ctx, x0->type, x->data.str, NULL);
    push_args(ctx, a);
    //int i;
    //FOR_EACH_ARRAY_INIT(a, x, i, i=2) {
    //    knh_dump(ctx, O(x));
    //}
    asm_expr(ctx, Array_n(a, 1));
    mtd->pc = CB->emit_code(CB);
    CB->optimize_code(CB, mtd);
    register_clear(CB, 0);
    CB = oldcb;
}
static void asm_return_expr(Ctx *ctx, knh_Token_t *x)
{
    knh_Token_t *x0 = cast(knh_Token_t*, x->data.o);
    Reg_t r = Arg0;
    if (x0) {
        r = asm_op2(ctx, x0);
    }
    CB->ret(CB, r);
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
        case FUNCTION_DECL:
            function_decl(ctx, x);
            break;
        case RETURN_NODE:
            asm_return_expr(ctx, x);
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

