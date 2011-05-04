#include "../vm.c"
#include "../array.h"
#include "test.h"
#define __(n) {(knh_int_t)n}
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
    vm_code_t *pc, code1[] = {
        {__(op_local_start), __(9), __(0), __(0), __(0)},
        {__(op_local_end),   __(9), __(0), __(0), __(0)},
        {__(op_ret),         __(0), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
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
    vm_delete(vm);
    return 1;
}

static int test_float_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
    knh_value_t n1, n2, n3;
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
    v1.fval = 10.0;v2.fval = 20.0;v3.fval = 30.0;
    _code_(0, v, ncall_test_v, 0, 0, 0, 0, dval);
    _code_(1, i, ncall_test_i, 10, 20, 30, 10, ival);
    _code_(2, f, ncall_test_f, v1.dval, v2.dval, v3.dval, v1.fval, fval);
    _code_(3, p, ncall_test_p, 10, 20, 30, 10, dval);
#undef _code_
    vm_delete(vm);
    return 1;
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
    if (ret != 20) {
        return 0;
    }
    vm->r.arg[0].ival = 100;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vm_delete(vm);
    return ret == 30;
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

static struct knh_Method_t *new_Method(fvm2 func, vm_code_t *pc)
{
    struct knh_Method_t *mtd = new_(Method);
    mtd->call = func;
    mtd->pc   = pc;
    return mtd;
}
static int fibo(int n)
{
    if (n < 3) {
        return 1;
    }
    return fibo(n-1) + fibo(n-2);
}

static int test_vm_fibo(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc = NULL;

    struct knh_Method_t *mtd = new_Method(NULL, NULL);
    struct label l1;
#define __N__ 10
    cb->nset_i(cb, Reg3, 3);
    cb->jilt(cb, &l1, Arg0, Reg3);
    cb->nset_i(cb, Reg1, 1);
    cb->ret(cb,  Reg1);
    l1.replaceLabelWith(&l1, cb);
#define LOCALSIZE 2
    cb->local_start(cb, LOCALSIZE);
    /* n - 1 */
    cb->nset_i(cb, Reg1, 1);
    cb->isub(cb, Reg2, Arg0, Reg1);
    cb->movlr(cb, 0, Reg2);
    /* n - 2 */
    cb->nset_i(cb, Reg1, 2);
    cb->isub(cb, Arg0, Arg0, Reg1);
    /* b = fibo(n-2) */
    cb->call(cb, Reg4, mtd, pc);
    cb->movlr(cb, 1, Reg4);
    /* a = fibo(n-1) */
    cb->movrl(cb, Arg0, 0);
    cb->call(cb, Reg5, mtd, pc);
    /* c = a + b */
    cb->movrl(cb, Reg4, 1);
    cb->iadd(cb, Reg2, Reg4, Reg5);
    cb->local_end(cb, LOCALSIZE);
    /* return c */
    cb->ret(cb,  Reg2);
    pc = cb->emit_code(cb);
    mtd->pc = pc;
    cb->optimize_code(cb, mtd);
    vm->r.arg[0].ival = __N__;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == fibo(__N__);
}
static knh_string_t __s = {10, "123456789"};
static int test_vm_nmov_omov(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    vm_code_t *pc;

    cb->nset_i(cb, Reg1, 20);
    cb->oset(cb, Reg2, (knh_Object_t*)&__s);
    cb->nmov(cb, Reg3, Reg1);
    cb->omov(cb, Reg4, Reg2);
    cb->ret(cb,  Reg3);
    pc = cb->emit_code(cb);
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    if (strcmp(((knh_string_t*)vm->r.reg[Reg4].o)->txt, __s.txt) != 0) {
        return false;
    }
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
    TESTCASE(test_vm_fibo),
    TESTCASE(test_vm_nmov_omov),
};

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))

int main(int argc, char **argv)
{
    __test__(__TESTCASE__, _ARRAY_SIZE(__TESTCASE__));
    return 0;
}

