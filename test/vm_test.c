#include "../vm.c"
#include "../array.h"
#include "test.h"
static inline void *__new(size_t size, knh_class_t cid)
{
    knh_Object_t *o = malloc_(size);
    o->h.magicflag  = MINIKONOHA_MAGIC;
    o->h.classinfo  = cid;
    return o;
}


knh_Array_t *new_Array(void)
{
    knh_Array_t *a = new_(Array);
    a->type = TYPE_UNTYPED;
    a->size = 0;
    a->capacity = 1;
    a->list = malloc_(sizeof(void*));
    return a;
}

static int test_iadd_nset(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    vm_code_t *pc, code[] = {
        {__(op_nset),        __(3), __(9), __(0), __(0)},
        {__(op_nset),        __(2), __(8), __(0), __(0)},
        {__(op_iadd),        __(1), __(2), __(3), __(0)},
        {__(op_ret),         __(1), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    pc = vm_code_init(vm, code);
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vm_delete(vm);
    return ret == (9+8);
}

static int test_fadd_nset(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();
    knh_value_t v1, v2;
    v1.fval = 10.0;
    v2.fval = 20.0;
    vm_code_t *pc, code[] = {
        {__(op_nset),        __(3), __(v1.dval), __(0), __(0)},
        {__(op_nset),        __(2), __(v2.dval), __(0), __(0)},
        {__(op_fadd),        __(1), __(2), __(3), __(0)},
        {__(op_ret),         __(1), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code);
    vm_exec(vm, pc);
    ret = vm->r.ret.fval;
    vm_delete(vm);
    return ret == (10.0 + 20.0);
}

static int test_local_start(void)
{
    vm_t *vm = vm_new();
    void *sp = vm->sp;
    int  ret;
#define __(n) {(knh_int_t)n}
    vm_code_t *pc, code1[] = {
        {__(op_local_start), __(9), __(0), __(0), __(0)},
        {__(op_local_end),   __(9), __(0), __(0), __(0)},
        {__(op_ret),         __(0), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code1);
    vm_exec(vm, pc);
    ret = (vm->sp == sp);
    vm_delete(vm);
    return ret;
}

static int test_icast_fcast(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();

    knh_value_t v1;
    v1.fval = 15.0;
#define __(n) {(knh_int_t)n}
    vm_code_t *pc, code1[] = {
        {__(op_nset),        __(3), __(v1.dval), __(0), __(0)},
        {__(op_icast),       __(2), __(3), __(0), __(0)},
        {__(op_ret),         __(2), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    vm_code_t code2[] = {
        {__(op_nset),        __(3), __(15), __(0), __(0)},
        {__(op_fcast),       __(2), __(3), __(0), __(0)},
        {__(op_ret),         __(2), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code1);
    vm_exec(vm, pc);
    if (vm->r.ret.ival != 15) {
        return 0;
    }
    pc = vm_code_init(vm, code2);
    vm_exec(vm, pc);
    ret = vm->r.ret.fval;
    vm_delete(vm);

    return ret == (15.0);
}

static int test_int_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
#define __(n) {(knh_int_t)n}
#define _code_(n, op, v1, v2, v3, v, rval) \
    vm_code_t code##n [] = {    \
        {__(op_nset),        __(2), __(v1), __(0), __(0)}, \
        {__(op_nset),        __(3), __(v2), __(0), __(0)}, \
        {__(op_nset),        __(4), __(v3), __(0), __(0)}, \
        {__(op_i##op),       __(1), __(2), __(3), __(0)},  \
        {__(op_i##op),       __(1), __(1), __(4), __(0)},  \
        {__(op_ret),         __(1), __(0), __(0), __(0)},  \
        {__(op_exit),        __(0), __(0), __(0), __(0)},  \
    };\
    pc = vm_code_init(vm, code##n);\
    vm_exec(vm, pc); \
    if (vm->r.ret.rval != (v)) return 0;

    _code_(0, add, 30, 40, 50, 30 + 40 + 50, ival);
    _code_(1, sub, 30, 40, 50, 30 - 40 - 50, ival);
    _code_(2, mul, 30, 40, 50, 30 * 40 * 50, ival);
    _code_(3, div, 30,  3,  5, 30 /  3 /  5, ival);
#undef _code_
#undef __
    vm_delete(vm);
    return 1;
}

static int test_float_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
    knh_value_t n1, n2, n3;
#define __(n) {(knh_int_t)n}
#define _code_(n, op, v1, v2, v3, v, rval) \
    vm_code_t code##n [] = {    \
        {__(op_nset),        __(2), __(v1), __(0), __(0)}, \
        {__(op_nset),        __(3), __(v2), __(0), __(0)}, \
        {__(op_nset),        __(4), __(v3), __(0), __(0)}, \
        {__(op_f##op),       __(1), __(2), __(3), __(0)},  \
        {__(op_f##op),       __(1), __(1), __(4), __(0)},  \
        {__(op_ret),         __(1), __(0), __(0), __(0)},  \
        {__(op_exit),        __(0), __(0), __(0), __(0)},  \
    };\
    pc = vm_code_init(vm, code##n);\
    vm_exec(vm, pc); \
    if (vm->r.ret.rval != (v)) return 0;
    n1.fval = 30.0; n2.fval = 40.0; n3.fval = 50.0;
    _code_(0, add, n1.dval, n2.dval, n3.dval, n1.fval + n2.fval + n3.fval, fval);
    _code_(1, sub, n1.dval, n2.dval, n3.dval, n1.fval - n2.fval - n3.fval, fval);
    _code_(2, mul, n1.dval, n2.dval, n3.dval, n1.fval * n2.fval * n3.fval, fval);
    _code_(3, div, n1.dval, n2.dval, n3.dval, n1.fval / n2.fval / n3.fval, fval);
#undef _code_
#undef __
    vm_delete(vm);
    return 1;
}

static void fcall_test(vm_t *vm)
{
    vm->r.ret.ival = vm->r.reg[2].ival + vm->r.reg[3].ival + vm->r.reg[4].ival;
}

static int test_fcall(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
#define __(n) {(knh_int_t)n}
    vm_code_t code [] = {
        {__(op_nset),        __(2), __(10), __(0), __(0)},
        {__(op_nset),        __(3), __(20), __(0), __(0)},
        {__(op_nset),        __(4), __(30), __(0), __(0)},
        {__(op_fcall),       __(1), __(0), __(fcall_test), __(0)},
        {__(op_ret),         __(1), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    pc = vm_code_init(vm, code);
    vm_exec(vm, pc);
    if (vm->r.ret.ival != 10 + 20 + 30) return 0;
#undef _code_
#undef __
    vm_delete(vm);
    return 1;
}

static void ncall_test_v(void)
{
}
static knh_int_t ncall_test_i(knh_int_t i)
{
    return i;
}
static knh_float_t ncall_test_f(knh_float_t f)
{
    return f;
}
static knh_Object_t *ncall_test_p(knh_Object_t *o)
{
    return o;
}

static int test_ncall(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
#define RARG(n) (VM_REG_SIZE + 1 + n)
#define __(n) {(knh_int_t)n}
#define _code_(n, op, f, v1, v2, v3, v, rval) \
    vm_code_t code##n [] = {\
        {__(op_nset),        __(RARG(0)), __(v1), __(0), __(0)},\
        {__(op_nset),        __(RARG(1)), __(v2), __(0), __(0)},\
        {__(op_nset),        __(RARG(2)), __(v3), __(0), __(0)},\
        {__(op_ncall_##op),  __(1), __(0),  __(f), __(0)},\
        {__(op_ret),         __(1), __(0),  __(0), __(0)},\
        {__(op_exit),        __(0), __(0),  __(0), __(0)},\
    };\
    pc = vm_code_init(vm, code##n);\
    vm_exec(vm, pc);\
    if (vm->r.ret.rval != v) return 0;
    knh_value_t v1, v2, v3;
    v1.fval = 10.0;v1.fval = 20.0;v1.fval = 30.0;
    _code_(0, v, ncall_test_v, 0, 0, 0, 0, dval);
    _code_(1, i, ncall_test_i, 10, 20, 30, 10, ival);
    _code_(2, f, ncall_test_f, v1.dval, v2.dval, v3.dval, v1.fval, fval);
    _code_(3, p, ncall_test_p, 10, 20, 30, 10, dval);
#undef _code_
#undef __
    vm_delete(vm);
    return 1;
}

typedef enum Reg_t {
    Reg0,Reg1,Reg2,Reg3,Reg4,Reg5,Reg6,Reg7,
    RegRet,
    Arg0,Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9,Arg10,Arg11,
} Reg_t;
struct vmcode_builder;
struct label {
    int index;
    void (*replaceLabelWith)(struct label*, struct vmcode_builder*);
};
typedef vm_code_t *knh_code_t;
DEF_ARRAY_S_T(code);
DEF_ARRAY_S_STRUCT(code);
DEF_ARRAY_S_OP(code);

struct vmcode_builder {
    vm_t *vm;
    Array(code) *codebuf;
    vm_code_t *(*emit_code)(struct vmcode_builder *);
    void (*nset_f)(struct vmcode_builder *, Reg_t, knh_float_t);
    void (*nset_i)(struct vmcode_builder *, Reg_t, knh_data_t);
    void (*nset)(struct vmcode_builder *, Reg_t, knh_value_t);
    void (*iadd)(struct vmcode_builder *, Reg_t, Reg_t, Reg_t);
    void (*isub)(struct vmcode_builder *, Reg_t, Reg_t, Reg_t);
    void (*imul)(struct vmcode_builder *, Reg_t, Reg_t, Reg_t);
    void (*idiv)(struct vmcode_builder *, Reg_t, Reg_t, Reg_t);
    void (*jilt)(struct vmcode_builder *, struct label *, Reg_t, Reg_t);
    void (*ret)(struct vmcode_builder *, Reg_t);
    void (*bcall)(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr);
    void (*call) (struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr);
    void (*scall)(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr);
    void (*fcall)(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr);
    void (*ncall)(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr);
};
static inline bool hasJump(vm_code_t *pc);
static vm_code_t *emit_code(struct vmcode_builder *cb)
{
    int i, size = Array_size(cb->codebuf) + 1;
    vm_code_t last_code = {(op_exit), (0), (0), (0), (0)};
    knh_code_t x, code = malloc_(sizeof(vm_code_t) * size);
    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        vm_code_t *xc = x;
        memcpy(code+i, xc, sizeof(vm_code_t));
        delete_(xc);
    }

    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        if (hasJump(x)) {
            int j;
            knh_code_t y;
            FOR_EACH_ARRAY(cb->codebuf, y, j) {
                if (j == x->a0.dval) {
                    //asm volatile("int3");
                    code[i].a0.ptr = code + j;
                    //fprintf(stderr, "i=%d j=%d %p %p\n", i, j, code[i].a0.ptr, code+i);
                }
            }
        }
    }

    memcpy(code+i, &last_code, sizeof(vm_code_t));
    delete_(cb->codebuf);
    code = vm_code_init(cb->vm, code);
    return code;
}
#define NEW_OP(op) (new_op(op_##op))
static inline vm_code_t *new_op(enum opcode op)
{
    vm_code_t *code = NEW(vm_code_t);
    code->c.code = op;
    return code;
}

static void nset_f(struct vmcode_builder *cb, Reg_t r, knh_float_t f)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.fval = f;
    Array_add(code, cb->codebuf, code);
}
static void nset_i(struct vmcode_builder *cb, Reg_t r, knh_data_t d)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.dval = d;
    Array_add(code, cb->codebuf, code);
}
static void nset(struct vmcode_builder *cb, Reg_t r, knh_value_t v)
{
    knh_code_t code = NEW_OP(nset);
    code->a0.ival = r;
    code->a1.dval = v.dval;
    Array_add(code, cb->codebuf, code);
}
static void isub(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3)
{
    knh_code_t code = NEW_OP(isub);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
    Array_add(code, cb->codebuf, code);
}
static void iadd(struct vmcode_builder *cb, Reg_t r1, Reg_t r2, Reg_t r3)
{
    knh_code_t code = NEW_OP(iadd);
    code->a0.ival = r1;
    code->a1.ival = r2;
    code->a2.ival = r3;
    Array_add(code, cb->codebuf, code);
}
#define Array_last(a) (Array_n(a, Array_size(a) - 1))
static void replaceLabelWith(struct label *l, struct vmcode_builder *cb)
{
    int i;
    vm_code_t *x;//, *last_code = Array_last(cb->codebuf);
    FOR_EACH_ARRAY(cb->codebuf, x, i) {
        if (x->a0.ptr == l || i == l->index) {
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
    Array_add(code, cb->codebuf, code);
}
static void ret(struct vmcode_builder *cb, Reg_t r)
{
    vm_code_t *code = NEW_OP(ret);
    code->a0.ival = r;
    Array_add(code, cb->codebuf, code);
}
static void bcall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(bcall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    Array_add(code, cb->codebuf, code);
}
static void call(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(call);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    Array_add(code, cb->codebuf, code);
}
static void scall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(scall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    Array_add(code, cb->codebuf, code);
}
static void fcall(struct vmcode_builder *cb, Reg_t r, struct knh_Method_t *mtd, void *ptr)
{
    vm_code_t *code = NEW_OP(fcall);
    code->a0.ival = r;
    code->a1.mtd  = (mtd);
    code->a2.ptr  = ptr;
    Array_add(code, cb->codebuf, code);
}
#define SETf(cb, op) cb->op = op
typedef struct vmcode_builder vmcode_builder;
static vmcode_builder *new_vmcode_builder(vm_t *vm)
{
    vmcode_builder *cb = cast(vmcode_builder*,malloc_(sizeof(vmcode_builder)));
    cb->vm = vm;
    SETf(cb, emit_code);
    SETf(cb, nset_i);
    SETf(cb, nset_f);
    SETf(cb, nset);
    SETf(cb, iadd);
    SETf(cb, jilt);
    SETf(cb, isub);
    SETf(cb, ret);
    SETf(cb, bcall);
    SETf(cb, scall);
    SETf(cb, fcall);
    SETf(cb, call);
    cb->codebuf = Array_new(code);
    return cb;
}
static vmcode_builder *vmcode_builder_init(struct vmcode_builder *cb)
{
    cb->codebuf = Array_new(code);
    return cb;
}

static void vmcode_builder_delete(struct vmcode_builder *cb)
{
    free_(cb);
}

static int test_vm_builder(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc;
    cb->nset_i(cb, Reg3, 9);
    cb->nset_i(cb, Reg2, 8);
    cb->iadd(cb, Reg2, Reg2, Reg3);
    cb->ret(cb,  Reg2);

    pc = cb->emit_code(cb);
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == (9+8);
}

static int test_vm_cond(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc;

    //  if (i < 10)  ret 20;
    //  else         ret 30;
    struct label l1;
    cb->nset_i(cb, Reg3, 10);
    cb->jilt(cb, &l1, Arg0, Reg3);
    cb->nset_i(cb, Reg1, 20);
    cb->ret(cb,  Reg1);
    l1.replaceLabelWith(&l1, cb);
    cb->nset_i(cb, Reg1, 30);
    cb->ret(cb,  Reg1);
    pc = cb->emit_code(cb);
    vm->r.arg[0].ival = 9;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == 20;
}

static int test_vm_bcall(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc;

    cb->nset_i(cb, Reg1, 20);
    cb->ret(cb,  Reg1);
    pc = cb->emit_code(cb);

    cb = vmcode_builder_init(cb);
    cb->bcall(cb, Reg4, NULL, pc);
    cb->ret(cb,  Reg4);
    pc = cb->emit_code(cb);
    vm->r.arg[0].ival = 9;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == 20;
}

static int test_vm_fibo(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc;

    knh_Method_t *mtd = new_Method(NULL, NULL);
    struct label l1;
    cb->nset_i(cb, Reg3, 3);
    cb->jilt(cb, &l1, Arg0, Reg3);
    cb->nset_i(cb, Reg1, 1);
    cb->ret(cb,  Reg1);
    l1.replaceLabelWith(&l1, cb);
    cb->nset_i(cb, Reg1, 1);
    cb->isub(cb, Reg2, Arg0, Reg1);
    cb->push(Reg2);
    cb->nset_i(cb, Reg1, 2);
    cb->isub(cb, Arg0, Arg0, Reg1);

    cb->call(cb, Reg4, mtd, pc);
    cb->call(cb, Reg4, mtd, pc);
    cb->ret(cb,  Reg4);
    pc = cb->emit_code(cb);
    vm->r.arg[0].ival = 9;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == 20;
}

static struct testcase __TESTCASE__[] = {
    TESTCASE(test_iadd_nset),
    TESTCASE(test_fadd_nset),
    TESTCASE(test_local_start),
    TESTCASE(test_icast_fcast),
    TESTCASE(test_int_op),
    TESTCASE(test_float_op),
    TESTCASE(test_fcall),
    TESTCASE(test_ncall),
    TESTCASE(test_vm_builder),
    TESTCASE(test_vm_cond),
    TESTCASE(test_vm_bcall),
};

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))

int main(int argc, char **argv)
{
    __test__(__TESTCASE__, _ARRAY_SIZE(__TESTCASE__));
    return 0;
}

