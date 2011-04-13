#ifndef TOKEN_H
#define TOKEN_H

enum token_code {
    STMT_LIST,
    TYPE_NODE,
    IDENTIFIER_NODE,
    INTEGER_CONST,
    FLOAT_CONST,
    STRING_CONST,
    PARAM_DECL,
    VAR_DECL,
    FUNCTION_DECL,
    CALL_EXPR,
    RETUEN_NODE, 
    TOKEN_CODE_MAX
};

enum operator_code {
    OpUndef  = TOKEN_CODE_MAX,
    OpEQLET  , /*    = */
    OpMULLET , /*   *= */
    OpDIVLET , /*   /= */
    OpMODLET , /*   %= */
    OpADDLET , /*   += */
    OpSUBLET , /*   -= */
    OpLSFTLET, /*  <<= */
    OpRSFTLET, /*  >>= */
    OpANDLET , /*   &= */
    OpXORLET , /*   ^= */
    OpORLET  , /*   |= */
    OpPlus   ,
    OpMinus  ,
    OpMul    ,
    OpDiv    ,
    OpMod    ,
    OpIn     ,
    OpLshift ,
    OpRshift ,
    OpOr     ,
    OpXor    ,
    OpAnd    ,
    OpNot    ,
    OpInv    ,
    OpLT     ,
    OpLE     ,
    OpGT     ,
    OpGE     ,
    OpEQ     ,
    OpNE     ,
    OpINC    ,
    OpDEC    ,
    OPERATOR_CODE_MAX
};

#define Token_CODE(t) (t->code)
#define Token_type(t) (t->type)
#define Token_isConst(o) (Token_CODE(o) == INTEGER_CONST || \
        Token_CODE(o) == FLOAT_CONST || \
        Token_CODE(o) == STRING_CONST)

#endif /* end of include guard: TOKEN_H */
