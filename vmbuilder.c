DEF_ARRAY_S_STRUCT(code);
DEF_ARRAY_S_OP(code);

DEF_ARRAY_S_STRUCT(Reg);
DEF_ARRAY_S_OP(Reg);
static inline bool hasJump(vm_code_t *pc);
static Reg_t regalloc(struct vmcode_builder *cb, intptr_t level)
{
    Reg_t r;
    if (Array_size(cb->regs) == 0) {
        Array_add(Reg, cb->regs, Reg0);
        return Reg0;
    }
    else if (Array_size(cb->regs) == Reg7) {
        asm volatile("int3");
        fprintf(stderr, "register spill\n");
    }
    r = Array_last(cb->regs) + 1;
    Array_add(Reg, cb->regs, r);
    return r;
}
static void regdealloc(struct vmcode_builder *cb, Reg_t r)
{
    if (Array_size(cb->regs) >= 0 && Array_size(cb->regs) <= r) {
        Array_n(cb->regs, r) = -1;
    }
}
static vm_code_t *emit_code(struct vmcode_builder *cb)
{
    int i, size = Array_size(cb->codebuf) + 2;
    vm_code_t last_code = {__(op_exit), __(0), __(0), __(0), __(0)};
    knh_code_t x, code = malloc_(sizeof(vm_code_t) * size);
    code = code + 1;
    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        vm_code_t *xc = x;
        memcpy(code+i, xc, sizeof(vm_code_t));
    }
    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        x = code + i;
        if (hasJump(x)) {
            int j;
            knh_code_t y;
            FOR_EACH_ARRAY(cb->codebuf, y, j) {
                if (j == x->a0.dval) {
                    //asm volatile("int3");
                    code[i].a0.ptr = code + j;
                    //fprintf(stderr, "i=%d j=%d %p %p\n",
                    //i, j, code[i].a0.ptr, code+i);
                }
            }
        }
    }

    memcpy(code+i, &last_code, sizeof(vm_code_t));

    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        delete_(x);
    }

    cb->codesize = size;
    delete_(cb->codebuf);
    code = vm_code_init(cb->vm, code);
    return code;
}
static inline void SetOpBCall(vm_code_t *pc);
static inline bool isOpCall(vm_code_t *pc);
#define OBJ(o) (cast(knh_Object_t*, o))
static void optimize_code(struct vmcode_builder *cb, struct knh_Method_t *mtd)
{
    int i, size = cb->codesize;
    knh_code_t x;
    for (i = 0; i < size; i++) {
        x = mtd->pc + i;
        if (isOpCall(x) && OBJ(x->a1.mtd)->h.classinfo == TYPE_Method) {
            knh_Method_t *mtd_ = x->a1.mtd;
            if (mtd_->pc) {
                SetOpBCall(x);
                x->a2.ptr = mtd_->pc + 1/*enter*/;
            }
        }
    }
}
#define NEW_OP(op) (new_op(op_##op))
static inline vm_code_t *new_op(enum opcode op)
{
    vm_code_t *code = NEW(vm_code_t);
    code->c.code = op;
    code->a0.ival = code->a1.ival = code->a2.ival = 0;
    return code;
}
static bool IS_int_op(intptr_t code)
{
    return (
        (code == op_iadd) ||
        (code == op_isub) ||
        (code == op_imul) ||
        (code == op_idiv));
}
static bool IS_int_cond(intptr_t code)
{
    return (
        (code == op_ieq) ||
        (code == op_ine) ||
        (code == op_igt) ||
        (code == op_ilt) ||
        (code == op_ige) ||
        (code == op_ile));
}
static bool IS_int_jmp(intptr_t code)
{
    return (
        (code == op_Jieq) ||
        (code == op_Jine) ||
        (code == op_Jigt) ||
        (code == op_Jilt) ||
        (code == op_Jige) ||
        (code == op_Jile));
}
#define int_const_op(code) (op_iaddC + code - op_iadd)
#define int_const_cond(code) (op_ieqC + code - op_ieq)
#define int_const_jmp(code) (op_JieqC + code - op_Jieq)
static void emit(Array(code) *codebuf, knh_code_t code)
{
    knh_code_t last_code;
    if (Array_size(codebuf) > 0) {
        last_code = Array_last(codebuf);
        if (last_code->c.code == op_nset && IS_int_op(code->c.code)) {
            if (last_code->a0.ival == code->a2.ival) {
                knh_data_t d = last_code->a1.dval;
                last_code->c.code = int_const_op(code->c.code);;
                last_code->a0.ival = code->a0.ival;
                last_code->a1.ival = code->a1.ival;
                last_code->a2.dval = d;
                delete_(code);
                return;
            }
        }
        if (last_code->c.code == op_nset && IS_int_cond(code->c.code)) {
            if (last_code->a0.ival == code->a2.ival) {
                knh_data_t d = last_code->a1.dval;
                struct label *l = (struct label*) code->a0.ptr;
                l->index -= 1;
                last_code->c.code = int_const_cond(code->c.code);;
                last_code->a0.ival = code->a0.ival;
                last_code->a1.ival = code->a1.ival;
                last_code->a2.dval = d;
                delete_(code);
                return;
            }
        }
        if (last_code->c.code == op_nset && IS_int_jmp(code->c.code)) {
            if (last_code->a0.ival == code->a2.ival) {
                knh_data_t d = last_code->a1.dval;
                last_code->c.code = int_const_jmp(code->c.code);;
                last_code->a0.ival = code->a0.ival;
                last_code->a1.ival = code->a1.ival;
                last_code->a2.dval = d;
                delete_(code);
                return;
            }
        }
        if (last_code->c.code == op_nset && code->c.code == op_ret) {
            if (last_code->a0.ival == code->a0.ival) {
                knh_data_t d = last_code->a1.dval;
                last_code->c.code = op_retv;
                last_code->a0.ival = d;
                delete_(code);
                return;
            }
        }

    }
    Array_add(code, codebuf, code);
}
static void nset_f(struct vmcode_builder *cb, Reg_t r, knh_float_t f)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.fval = f;
    emit(cb->codebuf, code);
}
static void nset_i(struct vmcode_builder *cb, Reg_t r, knh_data_t d)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.dval = d;
    emit(cb->codebuf, code);
}
static void nset(struct vmcode_builder *cb, Reg_t r, knh_value_t v)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.dval = v.dval;
    emit(cb->codebuf, code);
}
static void oset(struct vmcode_builder *cb, Reg_t r, knh_Object_t *o)
{
    knh_code_t code = NEW_OP(oset);
    code->a0.ival = r;
    code->a1.o    = o;
    emit(cb->codebuf, code);
}
static void local_start(struct vmcode_builder *cb, int idx)
{
    knh_code_t code = NEW_OP(local_start);
    code->a0.ival = idx;
    emit(cb->codebuf, code);
}
static void local_end(struct vmcode_builder *cb, int idx)
{
    knh_code_t code = NEW_OP(local_end);
    code->a0.ival = idx;
    emit(cb->codebuf, code);
}
static void movlr(struct vmcode_builder *cb, int idx, Reg_t r1)
{
    knh_code_t code = NEW_OP(movlr);
    code->a0.ival = idx;
    code->a1.ival = r1;
    Array_add(code, cb->codebuf, code);
}
static void movrl(struct vmcode_builder *cb, Reg_t r1, int idx)
{
    knh_code_t code = NEW_OP(movrl);
    code->a0.ival = r1;
    code->a1.ival = idx;
    emit(cb->codebuf, code);
}
static void nmov(struct vmcode_builder *cb, Reg_t r1, Reg_t r2)
{
    knh_code_t code = NEW_OP(nmov);
    code->a0.ival = r1;
    code->a1.ival = r2;
    emit(cb->codebuf, code);
}
static void omov(struct vmcode_builder *cb, Reg_t r1, Reg_t r2)
{
    knh_code_t code = NEW_OP(omov);
    code->a0.ival = r1;
    code->a1.ival = r2;
    emit(cb->codebuf, code);
}
static void replaceLabelWith(struct label *l, struct vmcode_builder *cb)
{
    int i;
    vm_code_t *x;
    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        if (hasJump(x) && (x->a0.ptr == l || i == l->index)) {
            x->a0.ival = Array_size(cb->codebuf);
        }
    }
}
static void emit_op2(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ival = r1;
    code->a1.ival = r2;
    emit(cb->codebuf, code);
}
static void emit_op3(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
    emit(cb->codebuf, code);
}
static void emit_cond_jmp(struct vmcode_builder *cb, struct label *l, Reg_t r1, Reg_t r2, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ptr = l;
    code->a1.ival = r1;
    code->a2.ival = r2;
    l->replaceLabelWith = replaceLabelWith;
    l->index = Array_size(cb->codebuf);
    emit(cb->codebuf, code);
}
#define EMIT_OP(__op__) \
static void __op__ (struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3) {\
    emit_op3(cb, r1, r2, r3, op_##__op__);\
}
#define EMIT_OP2(__op__) \
static void __op__ (struct vmcode_builder *cb, Reg_t r1, Reg_t r2) {\
    emit_op2(cb, r1, r2, op_##__op__);\
}
#define EMIT_COND_JMP_OP(__op__) \
static void __op__ (struct vmcode_builder *cb, struct label *l, Reg_t r1, Reg_t r2) {\
    emit_cond_jmp(cb, l, r1, r2, op_##__op__);\
}
EMIT_OP(iadd);
EMIT_OP(isub);
EMIT_OP(imul);
EMIT_OP(idiv);
EMIT_OP(imod);
EMIT_OP(ieq);
EMIT_OP(ine);
EMIT_OP(igt);
EMIT_OP(ilt);
EMIT_OP(ige);
EMIT_OP(ile);
EMIT_OP(fadd);
EMIT_OP(fsub);
EMIT_OP(fmul);
EMIT_OP(fdiv);
EMIT_OP(feq);
EMIT_OP(fne);
EMIT_OP(fgt);
EMIT_OP(flt);
EMIT_OP(fge);
EMIT_OP(fle);

EMIT_OP2(fpow);
EMIT_OP2(fcast);
EMIT_OP2(ipow);
EMIT_OP2(icast);

EMIT_COND_JMP_OP(Jieq);
EMIT_COND_JMP_OP(Jine);
EMIT_COND_JMP_OP(Jigt);
EMIT_COND_JMP_OP(Jilt);
EMIT_COND_JMP_OP(Jige);
EMIT_COND_JMP_OP(Jile);

EMIT_COND_JMP_OP(Jfeq);
EMIT_COND_JMP_OP(Jfne);
EMIT_COND_JMP_OP(Jfgt);
EMIT_COND_JMP_OP(Jflt);
EMIT_COND_JMP_OP(Jfge);
EMIT_COND_JMP_OP(Jfle);

static void emit_op_in(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
    emit(cb->codebuf, code);
}
static void emit_op_in_jmp(struct vmcode_builder *cb, struct label *l, Reg_t r1, Reg_t r2, Reg_t r3, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ptr  = (void*)l;
    code->a1.ival = r1;
    code->a2.ival = r2;
    code->a3.ival = r3;
    emit(cb->codebuf, code);
}
#define EMIT_OP_IN(__op__) \
static void __op__ (struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3) {\
    emit_op_in(cb, r1, r2, r3, op_##__op__);\
}
#define EMIT_OP_IN_JMP(__op__) \
static void __op__ (struct vmcode_builder *cb, struct label *l, Reg_t r1, Reg_t r2, Reg_t r3) {\
    emit_op_in_jmp(cb, l, r1, r2, r3, op_##__op__);\
}
EMIT_OP_IN(iin);
EMIT_OP_IN(inoin);
EMIT_OP_IN(fin);
EMIT_OP_IN(fnoin);
EMIT_OP_IN_JMP(Jiin);
EMIT_OP_IN_JMP(Jinoin);
EMIT_OP_IN_JMP(Jfin);
EMIT_OP_IN_JMP(Jfnoin);

static void ret(struct vmcode_builder *cb, Reg_t r)
{
    vm_code_t *code = NEW_OP(ret);
    code->a0.ival = r;
    emit(cb->codebuf, code);
}
#define EMIT_OP_CALL(__op__) \
static void __op__ (struct vmcode_builder *cb, Reg_t r1, struct knh_Method_t *mtd, void *ptr) {\
    emit_op_call(cb, r1, mtd, ptr, op_##__op__);\
}
static void emit_op_call(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr, enum opcode op)
{
    vm_code_t *code = new_op(op);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    emit(cb->codebuf, code);
}
EMIT_OP_CALL(scall);
EMIT_OP_CALL(call);
EMIT_OP_CALL(bcall);
EMIT_OP_CALL(fcall);
EMIT_OP_CALL(ncall_v);
EMIT_OP_CALL(ncall_i);
EMIT_OP_CALL(ncall_f);
EMIT_OP_CALL(ncall_p);

#define SETf(cb, op) cb->op = op
typedef struct vmcode_builder vmcode_builder;
struct vmcode_builder *new_vmcode_builder(vm_t *vm)
{
    vmcode_builder *cb = cast(vmcode_builder*,malloc_(sizeof(vmcode_builder)));
    cb->vm = vm;
    cb->regs = Array_new(Reg);
    SETf(cb, regalloc);
    SETf(cb, emit_code);
    SETf(cb, optimize_code);
    SETf(cb, local_start);
    SETf(cb, local_end);
    SETf(cb, movlr);
    SETf(cb, movrl);
    SETf(cb, nset_i);
    SETf(cb, nset_f);
    SETf(cb, nset);
    SETf(cb, nmov);
    SETf(cb, omov);
    SETf(cb, iadd);
    SETf(cb, isub);
    SETf(cb, iadd);
    SETf(cb, isub);
    SETf(cb, imul);
    SETf(cb, idiv);
    SETf(cb, imod);
    SETf(cb, ipow);
    SETf(cb, icast);
    SETf(cb, ieq);
    SETf(cb, ine);
    SETf(cb, igt);
    SETf(cb, ilt);
    SETf(cb, ige);
    SETf(cb, ile);
    SETf(cb, iin);
    SETf(cb, inoin);
    SETf(cb, Jieq);
    SETf(cb, Jine);
    SETf(cb, Jigt);
    SETf(cb, Jilt);
    SETf(cb, Jige);
    SETf(cb, Jile);
    SETf(cb, Jiin);
    SETf(cb, Jinoin);
    SETf(cb, fadd);
    SETf(cb, fsub);
    SETf(cb, fmul);
    SETf(cb, fdiv);
    SETf(cb, fpow);
    SETf(cb, fcast);
    SETf(cb, feq);
    SETf(cb, fne);
    SETf(cb, fgt);
    SETf(cb, flt);
    SETf(cb, fge);
    SETf(cb, fle);
    SETf(cb, fin);
    SETf(cb, fnoin);
    SETf(cb, Jfeq);
    SETf(cb, Jfne);
    SETf(cb, Jfgt);
    SETf(cb, Jflt);
    SETf(cb, Jfge);
    SETf(cb, Jfle);
    SETf(cb, Jfin);
    SETf(cb, Jfnoin);
    SETf(cb, ret);
    SETf(cb, bcall);
    SETf(cb, scall);
    SETf(cb, fcall);
    SETf(cb, ncall_v);
    SETf(cb, ncall_i);
    SETf(cb, ncall_f);
    SETf(cb, ncall_p);
    SETf(cb, call);
    SETf(cb, oset);
    cb->codebuf = Array_new(code);
    return cb;
}
struct vmcode_builder *vmcode_builder_init(struct vmcode_builder *cb)
{
    cb->codebuf = Array_new(code);
    cb->codesize = 0;
    return cb;
}

void vmcode_builder_delete(struct vmcode_builder *cb)
{
    free_(cb);
}

