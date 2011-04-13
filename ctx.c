

static void type_write(CTX ctx, FILE *fd, knh_class_t cid)
{
    struct type_info *typeinfo = ctx->types + cid;
    fprintf(fd, "%s", typeinfo->name);
    if (typeinfo->param) {
        int i;
        knh_class_t x;
        fputs("<", fd);
        FOR_EACH_ARRAY_INIT(typeinfo->param, x, i, i=1) {
            if (i != 1)
                fputs(",", fd);
            type_write(ctx, fd, x);
        }
        fputs("=>", fd);
        type_write(ctx, fd, Array_n(typeinfo->param, 0));
        fputs(">", fd);
    }
}

static void default_write(CTX ctx, FILE *fd, knh_Object_t *o)
{
    struct type_info *typeinfo = ctx->types + o->h.classinfo;
    fprintf(fd, "%s(%p)\n", typeinfo->name, o);
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

static void Token_write(CTX ctx, FILE *fd, knh_Object_t *o)
{
    knh_Token_t *t = cast(knh_Token_t*, o);
    enum token_code code = Token_CODE(t);
    fprintf(fd, "Token(");
    switch(code) {
        case STMT_LIST:
        {
            int i;
            knh_Token_t *x;
            fputs("stmt_list [", fd);
            FOR_EACH_TOKEN(t, x, i) {
                if (i != 0) fputs(", ", fd);
                Token_write(ctx, fd, O(x));
            }
            fputs("]", fd);
            break;
        }
        case TYPE_NODE:
            fprintf(fd, "type(");
            type_write(ctx, fd, Token_type(t));
            fprintf(fd, ")");
            break;
        case IDENTIFIER_NODE:
            fprintf(fd, "id:%s", t->data.str->txt);
            break;
        case INTEGER_CONST:
            fprintf(fd, "int:%d", t->data.ival);
            break;
        case FLOAT_CONST:
            fprintf(fd, "float:%f", t->data.fval);
            break;
        case STRING_CONST:
            fputs("string:", fd);
            break;
        case RETUEN_NODE:
            fputs("return:", fd);
            Token_write(ctx, fd, O(t->data.o));
            break;

        case FUNCTION_DECL:
        {
            Array(Token) *a = ((Array(Token)*)t->data.o);
            fputs("function_decl:", fd);
            Token_write(ctx, fd, O(Array_n(a, 0)));
            fputs(" ", fd);
            type_write(ctx, fd, Token_type(t));
            break;
        }
        case VAR_DECL:
        {
            Tuple(Token, Token) *tpl = (Tuple(Token, Token)*)t->data.o;
            fputs("var_decl:", fd);
            Token_write(ctx, fd, O(tpl->o1));
            fputs(":=", fd);
            Token_write(ctx, fd, O(tpl->o2));
            break;
        }
        case CALL_EXPR:
        {
            int i;
            knh_Token_t  *x;
            Array(Token) *a = ((Array(Token)*)t->data.o);
            fputs("call:", fd);
            FOR_EACH_ARRAY(a, x, i) {
                if (i == 0) {
                    Token_write(ctx, fd, O(x));
                    fputs("(", fd);
                }
                else {
                    if (i != 1) fputs(",", fd);
                    Token_write(ctx, fd, O(x));
                }
            }
            fputs(")", fd);
            break;
        }

        default:
            if (code >= TOKEN_CODE_MAX) {
                Array(Token) *a = ((Array(Token)*)t->data.o);
                Token_write(ctx, fd, O(Array_n(a, 0)));
                fputs(operator_code_string[code - TOKEN_CODE_MAX], fd);
                if (Array_size(a) > 1) {
                    Token_write(ctx, fd, O(Array_n(a, 1)));
                }
            }
            break;
    }
    fputs(")", fd);
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

static CTX new_context(void)
{
    struct context *ctx = malloc_(sizeof(struct context));
    construct_default_value((CTX)ctx);
    return (CTX)ctx;
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

