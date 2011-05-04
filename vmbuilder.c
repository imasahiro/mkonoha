DEF_ARRAY_S_STRUCT(code);
DEF_ARRAY_S_OP(code);

static inline bool hasJump(vm_code_t *pc);
static vm_code_t *emit_code(struct vmcode_builder *cb)
{
    int i, size = Array_size(cb->codebuf) + 2;
    vm_code_t last_code = {__(op_exit), __(0), __(0), __(0), __(0)};
    knh_code_t x, code = malloc_(sizeof(vm_code_t) * size);
    fprintf(stderr, "base ptr=%p, %p\n", code, code+1);
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
static void isub(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3)
{
    knh_code_t code = NEW_OP(isub);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
    emit(cb->codebuf, code);
}
static void iadd(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3)
{
    knh_code_t code = NEW_OP(iadd);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
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
static void jilt(struct vmcode_builder *cb, struct label *l, Reg_t r1, Reg_t r2)
{
    vm_code_t *code = NEW_OP(Jilt);
    code->a0.ptr = l;
    code->a1.ival = r1;
    code->a2.ival = r2;
    l->replaceLabelWith = replaceLabelWith;
    l->index = Array_size(cb->codebuf);
    emit(cb->codebuf, code);
}
static void ret(struct vmcode_builder *cb, Reg_t r)
{
    vm_code_t *code = NEW_OP(ret);
    code->a0.ival = r;
    emit(cb->codebuf, code);
}
static void bcall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(bcall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    emit(cb->codebuf, code);
}
static void call(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(call);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    emit(cb->codebuf, code);
}
static void scall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(scall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    emit(cb->codebuf, code);
}
static void fcall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(fcall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    emit(cb->codebuf, code);
}
#define SETf(cb, op) cb->op = op
typedef struct vmcode_builder vmcode_builder;
struct vmcode_builder *new_vmcode_builder(vm_t *vm)
{
    vmcode_builder *cb = cast(vmcode_builder*,malloc_(sizeof(vmcode_builder)));
    cb->vm = vm;
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
    SETf(cb, jilt);
    SETf(cb, isub);
    SETf(cb, ret);
    SETf(cb, bcall);
    SETf(cb, scall);
    SETf(cb, fcall);
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

