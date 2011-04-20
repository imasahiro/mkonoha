#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "konoha.h"
#include "memory.h"
#include "vmop.h"

typedef knh_value_t value_t;
typedef struct knh_ObjectField_t knh_ObjectField_t;
DEF_ARRAY_S_T(data);
DEF_ARRAY_S_STRUCT(data);
DEF_ARRAY_S_OP(data);

DEF_ARRAY_STRUCT(Object);
DEF_ARRAY_OP(Object);

typedef struct {
    union {
        intptr_t code;
        void *addr;
    } c;
    value_t a0;
    value_t a1;
    value_t a2;
    value_t a3;
} vm_code_t;

#define VM_MAX_REG_SIZE 16
#define VM_REG_SIZE     8
typedef struct vm_t {
    union {
        value_t regs_[VM_MAX_REG_SIZE];
        struct {
            value_t reg[8];
            value_t ret;
            value_t arg[VM_MAX_REG_SIZE - 8 - 1];
        } r;
    };
    void *sp;
} vm_t;

typedef void (*fvm)(vm_t *);
typedef void (*fvm2)(vm_t *, vm_code_t *);
typedef knh_Object_t *(*fcast)(knh_Object_t *o1, knh_int_t v);
typedef void (*fa1_v)(void);
typedef knh_int_t     (*fa1_i)(knh_int_t);
typedef knh_float_t   (*fa1_f)(knh_float_t);
typedef knh_Object_t *(*fa1_p)(knh_Object_t*);
struct knh_Method_t {
    knh_hObject_t h;
    fvm2 call;
    vm_code_t *pc;
};




static void vm_exec(vm_t *vm, vm_code_t *pc);

static vm_t *vm_new(void)
{
    vm_t *vm = malloc_(sizeof(vm_t));
    int i;
    for (i = 0; i < VM_MAX_REG_SIZE; i++) {
        vm->regs_[i].o = NULL;
    }
    vm->r.ret.dval = 0;
    return vm;
}

static void vm_delete(vm_t *vm)
{
    free_(vm);
}

static vm_code_t *vm_code_init(vm_t *vm, vm_code_t *code)
{
    enum opcode opstart = code[1].c.code;
    vm_exec(vm, code);
    code[1].c.code = opstart;
    return code+1;
}

#ifndef KONOHA_VM
#include "vm_test.c"
#endif

#define _OFF_I(idx) (idx.i)
#define _OFF_X(idx) (idx.x)

static void VM_DBG_P(const char *f, vm_code_t *pc)
{
#if 0
    fprintf(stderr, "%s p pc=%p\n", f, pc);
#endif
}
static void VM_DBG_N(const char *f, vm_code_t *pc)
{
#if 0
    fprintf(stderr, "%s n pc=%p\n", f, pc);
#endif
}
#define L(l)    L_op_##l : asm("##" #l);
#define __a0(f) f()
#define __a1(f) f(pc->a0)
#define __a2(f) f(pc->a0, pc->a1)
#define __a3(f) f(pc->a0, pc->a1, pc->a2)
#define __a4(f) f(pc->a0, pc->a1, pc->a2, pc->a3)
#define _arg0(f) VM_DBG_P(#f, pc);__a0(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg1(f) VM_DBG_P(#f, pc);__a1(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg2(f) VM_DBG_P(#f, pc);__a2(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg3(f) VM_DBG_P(#f, pc);__a3(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg4(f) VM_DBG_P(#f, pc);__a4(f); VM_DBG_N(#f, pc); DISPATCH1(pc)

#undef O
#define A(i)         (pc->a ## i)
#define NARG(idx)    (arg[idx].ival)
#define FARG(idx)    (arg[idx].fval)
#define OARG(idx)    (arg[idx].o)
#define RET(vm)    (vm->r.ret.o)
#define RETv(vm)   (vm->r.ret.dval)
#define V(idx)     ((r[A(idx).ival]).dval)
#define N(idx)     ((r[A(idx).ival]).ival)
#define O(idx)     ((r[A(idx).ival]).o)
#define NX(idx)    (r[_OFF_I((A(idx).index))].ox->ox[_OFF_X((A(idx).index))].ival)
#define OX(idx)    (r[_OFF_I((A(idx).index))].ox->ox[_OFF_X((A(idx).index))].o)
#define F(idx)     ((r[A(idx).ival]).fval)
#define P(idx)     ((r[A(idx).ival]).o)
#define MTD(idx)   (r[A(idx).ival]).mtd
#define FUNC(idx)  ((fvm2)(r[A(idx).ival]).o)
#define FCAST(idx) ((fcast)(r[A(idx).ival]).o)
#define NC(idx)    (A(idx).ival)
#define OC(idx)    (A(idx).o)
#define C(idx)     ((knh_class_t)A(idx).ival)
#define NA(idx)    ((Array(data)*)  r[A(idx).ival].o)
#define OA(idx)    ((Array(Object)*) r[A(idx).ival].o)
#define jmp(add)   goto *(add)
static knh_Object_t *new_Object_(knh_class_t cid)
{
    return NULL;
}
static knh_Object_t *new_box(knh_class_t cid, knh_int_t i)
{
    return NULL;
}
knh_int_t new_unbox(knh_class_t cid, knh_Object_t *o)
{
    return 0;
}
static void _halt(void)
{
}

static void** THCODE__;
void __thcode_init(vm_code_t *pc, void **thcode)
{
    THCODE__ = thcode;
    while (pc->c.code != op_exit) {
        pc->c.addr = thcode[pc->c.code];
        pc++;
    }
    pc->c.addr = thcode[pc->c.code];
}

#define DISPATCH(pc)  goto *(THCODE[pc->c.code])
#define DISPATCH1(pc) ++pc;goto *(pc->c.addr)
#define vm_local_new(vm, n)    vm->sp += n;
#define vm_local_delete(vm, n) vm->sp -= n;

#include "vmcode.c"

