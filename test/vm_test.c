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
    vm_code_t *pc;
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    cb->nset_i(cb, Reg3, 9);
    cb->nset_i(cb, Reg2, 8);
    cb->iadd(cb, Reg1, Reg2, Reg3);
    cb->ret(cb, Reg1);
    pc = cb->emit_code(cb);
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == (9+8);
}

static int test_fadd_nset(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();
    vm_code_t *pc;
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    cb->nset_f(cb, Reg3, 10.0);
    cb->nset_f(cb, Reg2, 20.0);
    cb->fadd(cb, Reg1, Reg2, Reg3);
    cb->ret(cb, Reg1);
    pc = cb->emit_code(cb);

    vm_exec(vm, pc);
    ret = vm->r.ret.fval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == (10.0 + 20.0);
}

static int test_local_start(void)
{
    vm_t *vm = vm_new();
    void *sp = vm->sp;
    int  ret;
    struct vmcode_builder *cb;
    vm_code_t *pc;
    cb = new_vmcode_builder(vm);
    cb->local_start(cb, 9);
    cb->local_end(cb, 9);
    cb->ret(cb, Reg0);
    pc = cb->emit_code(cb);
    ret = (vm->sp == sp);
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret;
}

static int test_icast_fcast(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();
    vm_code_t *pc;
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    cb->nset_f(cb, Reg3, 15.0);
    cb->icast(cb, Reg2, Reg3);
    cb->nset_i(cb, Reg4, 15);
    cb->fcast(cb, Reg5, Reg4);
    cb->ret(cb, Reg2);
    pc = cb->emit_code(cb);
    vm_exec(vm, pc);
    if (vm->r.reg[Reg5].fval != (knh_float_t)15) {
        return 0;
    }
    ret = vm->r.ret.ival;
    vmcode_builder_delete(cb);
    vm_delete(vm);

    return ret == (15);
}

static int test_int_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
#define _code_(n, op, v1, v2, v3, v, rval) \
    cb = vmcode_builder_init(cb);\
    cb->nset_i(cb, Reg2, v1);\
    cb->nset_i(cb, Reg3, v2);\
    cb->nset_i(cb, Reg4, v3);\
    cb->i##op (cb, Reg1, Reg2, Reg3);\
    cb->i##op (cb, Reg1, Reg1, Reg4);\
    cb->ret(cb, Reg1);\
    pc = cb->emit_code(cb);\
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
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
#define _code_(n, op, v1, v2, v3, v, rval) \
    cb = vmcode_builder_init(cb);\
    cb->nset_f(cb, Reg2, v1);\
    cb->nset_f(cb, Reg3, v2);\
    cb->nset_f(cb, Reg4, v3);\
    cb->f##op (cb, Reg1, Reg2, Reg3);\
    cb->f##op (cb, Reg1, Reg1, Reg4);\
    cb->ret(cb, Reg1);\
    pc = cb->emit_code(cb);\
    vm_exec(vm, pc); \
    if (vm->r.ret.rval != (v)) return 0;
    _code_(0, add, 30.0, 40.0, 50.0, 30.0 + 40.0 + 50.0, fval);
    _code_(1, sub, 30.0, 40.0, 50.0, 30.0 - 40.0 - 50.0, fval);
    _code_(2, mul, 30.0, 40.0, 50.0, 30.0 * 40.0 * 50.0, fval);
    _code_(3, div, 30.0, 40.0, 50.0, 30.0 / 40.0 / 50.0, fval);
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
    struct vmcode_builder *cb;
    cb = new_vmcode_builder(vm);
    cb->nset_i(cb, Reg2, 10);
    cb->nset_i(cb, Reg3, 20);
    cb->nset_i(cb, Reg4, 30);
    cb->fcall(cb,  Reg1, NULL, (void*)fcall_test);
    cb->ret(cb, Reg1);
    pc = cb->emit_code(cb);
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
    struct vmcode_builder *cb;
    knh_value_t v1, v2, v3;
    cb = new_vmcode_builder(vm);
#define RARG(n) (VM_REG_SIZE + 1 + n)
#define _code_(n, op, f, v1, v2, v3, v, rval) \
    cb = vmcode_builder_init(cb);\
    cb->nset_i(cb, RARG(0), v1);\
    cb->nset_i(cb, RARG(1), v2);\
    cb->nset_i(cb, RARG(2), v3);\
    cb->ncall_##op (cb, Reg1, NULL, (void*)f);\
    cb->ret(cb, Reg1);\
    pc = cb->emit_code(cb);\
    vm_exec(vm, pc);\
    if (vm->r.ret.rval != v) return 0;
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
    cb->Jilt(cb, &l1, Arg0, Reg3);
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

    struct knh_Method_t *mtd1 = new_Method(NULL, NULL);
    struct knh_Method_t *mtd2 = new_Method(NULL, NULL);
    cb->nset_i(cb, Reg1, 20);
    cb->ret(cb,  Reg1);
    pc = cb->emit_code(cb);
    mtd1->pc = pc;

    cb = vmcode_builder_init(cb);
    cb->call(cb, Reg4, mtd1, pc);
    cb->ret(cb,  Reg4);
    pc = cb->emit_code(cb);
    mtd2->pc = pc;

    cb->optimize_code(cb, mtd2);
    vm->r.arg[0].ival = 9;
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vmcode_builder_delete(cb);
    vm_delete(vm);
    return ret == 20;
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
#ifndef __N__
#define __N__ 3
#endif
    cb->nset_i(cb, Reg3, 3);
    cb->Jilt(cb, &l1, Arg0, Reg3);
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
    TESTCASE(test_vm_builder),
    TESTCASE(test_vm_cond),
    TESTCASE(test_vm_bcall),
    TESTCASE(test_vm_fibo),
    TESTCASE(test_vm_nmov_omov),
    TESTCASE(test_iadd_nset),
    TESTCASE(test_fadd_nset),
    TESTCASE(test_local_start),
    TESTCASE(test_icast_fcast),
    TESTCASE(test_int_op),
    TESTCASE(test_float_op),
    TESTCASE(test_fcall),
    TESTCASE(test_ncall),

};

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))

int main(int argc, char **argv)
{
    __test__(__TESTCASE__, _ARRAY_SIZE(__TESTCASE__));
    return 0;
}

