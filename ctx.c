

static void type_write(CTX ctx, FILE *fd, knh_class_t cid)
{
    struct type_info *typeinfo = ctx->types + cid;
    fprintf(fd, "%s", typeinfo->name);
}

static void default_write(CTX ctx, FILE *fd, knh_Object_t *o)
{
    struct type_info *typeinfo = ctx->types + o->h.classinfo;
    fprintf(fd, "%s(%p)\n", typeinfo->name, o);
}

#define Token_CODE(t) (t->code)
#define Token_type(t) (t->type)
static void Token_write(CTX ctx, FILE *fd, knh_Object_t *o)
{
    knh_Token_t *t = cast(knh_Token_t*, o);
    enum token_code  code = Token_CODE(t);
    fprintf(fd, "Token(");
    switch(code) {
        case STMT_LIST:
            fprintf(fd, "stmt_list");
            break;
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
            fprintf(fd, "stmt_list");
            break;
        default:
            break;
    }
    fprintf(stderr, ")\n");
};


struct type_info DEFAULT_OBJECT_INFO[] = {
    {"Object" , TYPE_Object , TYPE_Object , TYPE_Object, default_write},
    {"Int"    , TYPE_Integer, TYPE_Integer, TYPE_Object, default_write},
    {"Float"  , TYPE_Float  , TYPE_Float  , TYPE_Object, default_write},
    {"String" , TYPE_String , TYPE_String , TYPE_Object, default_write},
    {"Token"  , TYPE_Token  , TYPE_Token  , TYPE_Object, Token_write},
    {"Class"  , TYPE_Class  , TYPE_Class  , TYPE_Object, default_write},
    {"Method" , TYPE_Method , TYPE_Method , TYPE_Object, default_write},
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
        memcpy(info, DEFAULT_OBJECT_INFO, sizeof(struct alias_info) * alias_size);
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

