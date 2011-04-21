#include "../vm.c"
#include "test.h"

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
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    vm_code_t code2[] = {
        {__(op_local_end),   __(9), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code1);
    vm_exec(vm, pc);
    if (vm->sp != (void*)((intptr_t)sp + 9)) {
        return 0;
    }
    pc = vm_code_init(vm, code2);
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

static struct testcase __TESTCASE__[] = {
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

