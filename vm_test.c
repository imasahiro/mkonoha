#include "test/test.h"

int test_iadd_nset(void)
{
    knh_data_t ret;
    vm_t *vm = vm_new();
#define __(n) {(knh_int_t)n}
    vm_code_t *pc, code[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
        {__(op_nset),        __(3), __(9), __(0), __(0)},
        {__(op_nset),        __(2), __(8), __(0), __(0)},
        {__(op_iadd),        __(1), __(2), __(3), __(0)},
        {__(op_ret),         __(1), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code);
    vm_exec(vm, pc);
    ret = vm->r.ret.dval;
    vm_delete(vm);
    return ret == (9+8);
}

int test_fadd_nset(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();
#define __(n) {(knh_int_t)n}
    knh_value_t v1, v2;
    v1.fval = 10.0;
    v2.fval = 20.0;
    vm_code_t *pc, code[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
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

int test_local_start(void)
{
    vm_t *vm = vm_new();
    void *sp = vm->sp;
    int  ret;
#define __(n) {(knh_int_t)n}
    vm_code_t *pc, code1[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
        {__(op_local_start), __(9), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    vm_code_t code2[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
        {__(op_local_end),   __(9), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
#undef __
    pc = vm_code_init(vm, code1);
    vm_exec(vm, pc);
    if (vm->sp != sp + 9) {
        return 0;
    }
    pc = vm_code_init(vm, code2);
    vm_exec(vm, pc);
    ret = (vm->sp == sp);
    vm_delete(vm);
    return ret;
}
int test_icast_fcast(void)
{
    knh_float_t ret;
    vm_t *vm = vm_new();

    knh_value_t v1;
    v1.fval = 15.0;
#define __(n) {(knh_int_t)n}
    vm_code_t *pc, code1[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
        {__(op_nset),        __(3), __(v1.dval), __(0), __(0)},
        {__(op_icast),       __(2), __(3), __(0), __(0)},
        {__(op_ret),         __(2), __(0), __(0), __(0)},
        {__(op_exit),        __(0), __(0), __(0), __(0)},
    };
    vm_code_t code2[] = {
        {__(op_thcode),      __(0), __(0), __(0), __(0)},
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

int test_int_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
#define __(n) {(knh_int_t)n}
#define _code_(n, op, v1, v2, v3, v, rval) \
    vm_code_t code##n [] = {    \
        {__(op_thcode),      __(0), __(0), __(0), __(0)},  \
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

int test_float_op(void)
{
    vm_t *vm = vm_new();
    vm_code_t *pc;
    knh_value_t n1, n2, n3;
#define __(n) {(knh_int_t)n}
#define _code_(n, op, v1, v2, v3, v, rval) \
    vm_code_t code##n [] = {    \
        {__(op_thcode),      __(0), __(0), __(0), __(0)},  \
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

static struct testcase __TESTCASE__[] = {
    TESTCASE(test_iadd_nset),
    TESTCASE(test_fadd_nset),
    TESTCASE(test_local_start),
    TESTCASE(test_icast_fcast),
    TESTCASE(test_int_op),
    TESTCASE(test_float_op),
};

#define _ARRAY_SIZE(a) ((int)(sizeof(a) / sizeof((a)[0])))

int main(int argc, char **argv)
{
    __test__(__TESTCASE__, _ARRAY_SIZE(__TESTCASE__));
    return 0;
}

