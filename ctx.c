

static void type_write(CTX ctx, struct io *io, knh_class_t cid)
{
    struct type_info *typeinfo = ctx->types + cid;
    io_printf(io, 1, "%s", typeinfo->name);
    if (typeinfo->param) {
        int i;
        knh_class_t x;
        io_puts(io, "<", 1);
        FOR_EACH_ARRAY_INIT(typeinfo->param, x, i, i=1) {
            if (i != 1)
                io_puts(io, ",", 1);
            type_write(ctx, io, x);
        }
        io_puts(io, "=>", 2);
        type_write(ctx, io, Array_n(typeinfo->param, 0));
        io_puts(io, ">", 1);
    }
}

static void default_write(CTX ctx, struct io *io, knh_Object_t *o)
{
    struct type_info *typeinfo = ctx->types + o->h.classinfo;
    io_printf(io, 2, "%s(%p)\n", typeinfo->name, o);
}
static const char * operator_code_string[] = {
    "undef",
    "=",
    "*=",
    "/=",
    "%=",
    "+=",
    "-=",
    "<<=",
    ">>=",
    "&=",
    "^=",
    "|=",
    "+",
    "-",
    "*",
    "/",
    "%",
    "in",
    "<<",
    ">>",
    "|",
    "^",
    "&",
    "!",
    "~",
    "<",
    "<=",
    ">", 
    ">=",
    "==",
    "!=",
    "++",
    "--",
};

#define io_puts2(io, ctxt) io_puts(io, ctxt, \
        __builtin_constant_p(ctxt)?sizeof(ctxt):strlen(ctxt))
static void Token_write(CTX ctx, struct io *io, knh_Object_t *o)
{
    knh_Token_t *t = cast(knh_Token_t*, o);
    enum token_code code = Token_CODE(t);
    io_puts2(io, "Token(");
    switch(code) {
        case STMT_LIST:
        {
            int i;
            knh_Token_t *x;
            io_puts2(io, "stmt_list [");
            FOR_EACH_TOKEN(t, x, i) {
                if (i != 0) io_puts2(io, ", ");
                Token_write(ctx, io, O(x));
            }
            io_puts2(io, "]");
            break;
        }
        case TYPE_NODE:
            io_puts2(io, "type(");
            type_write(ctx, io, Token_type(t));
            io_puts2(io, ")");
            break;
        case IDENTIFIER_NODE:
            io_printf(io, 1, "id:%s", t->data.str->txt);
            break;
        case INTEGER_CONST:
            io_printf(io, 1, "int:%lld", t->data.ival);
            break;
        case FLOAT_CONST:
            io_printf(io, 1, "float:%f", t->data.fval);
            break;
        case STRING_CONST:
            io_puts2(io, "string:");
            break;
        case RETUEN_NODE:
            io_puts2(io, "return:");
            Token_write(ctx, io, O(t->data.o));
            break;

        case FUNCTION_DECL:
        {
            Array(Token) *a = ((Array(Token)*)t->data.o);
            io_puts2(io, "function_decl:");
            Token_write(ctx, io, O(Array_n(a, 0)));
            io_puts2(io, " ");
            type_write(ctx, io, Token_type(t));
            break;
        }
        case VAR_DECL:
        {
            Tuple(Token, Token) *tpl = (Tuple(Token, Token)*)t->data.o;
            io_puts2(io, "var_decl:");
            Token_write(ctx, io, O(tpl->o1));
            io_puts2(io, ":=");
            Token_write(ctx, io, O(tpl->o2));
            break;
        }
        case CALL_EXPR:
        {
            int i;
            knh_Token_t  *x;
            Array(Token) *a = ((Array(Token)*)t->data.o);
            io_puts2(io, "call:");
            FOR_EACH_ARRAY(a, x, i) {
                if (i == 0) {
                    Token_write(ctx, io, O(x));
                    io_puts2(io,"(");
                }
                else {
                    if (i != 1) io_puts2(io, ",");
                    Token_write(ctx, io, O(x));
                }
            }
            io_puts2(io, ")");
            break;
        }

        default:
            if (code >= TOKEN_CODE_MAX) {
                Array(Token) *a = ((Array(Token)*)t->data.o);
                Token_write(ctx, io, O(Array_n(a, 0)));
                io_puts2(io,operator_code_string[code - TOKEN_CODE_MAX]);
                if (Array_size(a) > 1) {
                    Token_write(ctx, io, O(Array_n(a, 1)));
                }
            }
            break;
    }
    io_puts2(io, ")");
}


struct type_info DEFAULT_OBJECT_INFO[] = {
    {"Object" , TYPE_Object , TYPE_Object , TYPE_Object, NULL, default_write},
    {"Int"    , TYPE_Integer, TYPE_Integer, TYPE_Object, NULL, default_write},
    {"Float"  , TYPE_Float  , TYPE_Float  , TYPE_Object, NULL, default_write},
    {"String" , TYPE_String , TYPE_String , TYPE_Object, NULL, default_write},
    {"Token"  , TYPE_Token  , TYPE_Token  , TYPE_Object, NULL, Token_write},
    {"Class"  , TYPE_Class  , TYPE_Class  , TYPE_Object, NULL, default_write},
    {"Method" , TYPE_Method , TYPE_Method , TYPE_Object, NULL, default_write},
};

struct alias_info DEFAULT_ALIAS_INFO[] = {
    {"int"   , "Int"},
    {"float" , "Float"},
    {"double", "Float"},
};

static void construct_default_value(CTX ctx)
{
    {
        int typeinfo_size = _ARRAY_SIZE(DEFAULT_OBJECT_INFO);
        struct type_info *info  = malloc_(sizeof(struct type_info) * typeinfo_size);
        memcpy(info, DEFAULT_OBJECT_INFO, sizeof(struct type_info) * typeinfo_size);
        ((struct context *)ctx)->types     = info;
        ((struct context *)ctx)->type_size = typeinfo_size;
    }

    {
        int alias_size = _ARRAY_SIZE(DEFAULT_ALIAS_INFO);
        struct alias_info *info = malloc_(sizeof(struct alias_info) * alias_size);
        memcpy(info, DEFAULT_ALIAS_INFO,  sizeof(struct alias_info) * alias_size);
        ((struct context *)ctx)->alias      = info;
        ((struct context *)ctx)->alias_size = alias_size;
    }
}
static void context_init_stream(struct context *ctx)
{
    ((struct context *)ctx)->in  = new_io("/dev/stdin",  "r", 0);
    ((struct context *)ctx)->out = new_io("/dev/stdout", "w", 0);
    ((struct context *)ctx)->err = new_io("/dev/stderr", "w", 0);

}
static Ctx *new_context(void)
{
    struct context *ctx = malloc_(sizeof(struct context));
    construct_default_value((CTX)ctx);
    stream_init();
    context_init_stream(ctx);
    return ctx;
}

static void push_decl(Ctx *ctx, Tuple(Token, Token) *t)
{
    Array(Tuple) *a = ctx->decl_list;
    if (!a) {
        a = Array_new(Tuple);
        ctx->decl_list = a;
    }
    Array_add(Tuple, a, T(t));
}

static knh_class_t append_new_class(knh_class_t bcid, Array(class) *param)
{
    int i;
    Ctx *ctx = g_ctx;
    struct type_info *x;
    for (i = 0; i < ctx->type_size; i++) {
        x = ctx->types + i;
        if (x->bcid == bcid) {
            if (x->param && Array_eq(class, x->param, param)) {
                return x->cid;
            }
        }
    }

    {
        struct type_info *info;
        struct context *ctx = g_ctx;
        knh_class_t newcid = ctx->type_size;
        int size = ctx->type_size + 1;
        x = TYPEINFO_(ctx, bcid);
        ctx->types = realloc_(ctx->types, sizeof(struct type_info) * size);
        ctx->type_size = size;
        info = ctx->types+newcid;

        info->name = copy_(const char*, x->name, strlen(x->name)+1);
        info->suppercid = info->suppercid;
        info->cid   = newcid;
        info->bcid  = x->cid;
        info->param = param;
        info->write_ = x->write_;
        return newcid;
    }
}

