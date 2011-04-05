#ifndef TOKEN_H
#define TOKEN_H

enum token_code {
    STMT_LIST,
    TYPE_NODE,
    IDENTIFIER_NODE,
    INTEGER_CONST,
    FLOAT_CONST,
    STRING_CONST,
    VAR_DECL, 
    TOKEN_CODE_MAX
};

enum operator_code {
    OpUndef  ,
    OpEQLET  , /*    = */
    OpMULLET , /*   *= */
    OpDIVLET , /*   /= */
    OpMODLET , /*   %= */
    OpADDLET , /*   += */
    OpSUBLET , /*   -= */
    OpLSFTLET, /*  <<= */
    OpRSFTLET, /*  >>= */
    OpSHFTLET, /* >>>= */
    OpANDLET , /*   &= */
    OpXORLET , /*   ^= */
    OpORLET  , /*   |= */
    OpPlus   ,
    OpMinus  ,
    OpMul    ,
    OpDiv    ,
    OpMod    ,
    OpLshift ,
    OpRshift ,
    OpOr     ,
    OpXor    ,
    OpAnd    ,
    OpNot    ,
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


#endif /* end of include guard: TOKEN_H */
