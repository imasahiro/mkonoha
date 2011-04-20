#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "konoha.h"
#include "memory.h"
#include "vmop.h"

typedef knh_value_t value_t;
DEF_ARRAY_S_T(data);
DEF_ARRAY_S_STRUCT(data);
DEF_ARRAY_S_OP(data);

DEF_ARRAY_STRUCT(Object);
DEF_ARRAY_OP(Object);

typedef struct {
    union {
        void *addr;
        intptr_t code;
    };
    value_t a0;
    value_t a1;
    value_t a2;
    value_t a3;
} vm_code_t;

#define VM_MAX_REG_SIZE 16
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

enum {
    op_halt,
    op_exit,
    op_local_start,
    op_local_end,
    op_nmov,
    op_nmovx,
    op_xnmov,
    op_xnmovx,
    op_omov,
    op_omovx,
    op_xomov,
    op_xomovx,
    op_nset,
    op_xnset,
    op_oset,
    op_xoset,
    op_new,
    op_box,
    op_unbox,
    op_iadd,
    op_isub,
    op_imul,
    op_idiv,
    op_imod,
    op_icast,
    op_ieq,
    op_ine,
    op_igt,
    op_ilt,
    op_ige,
    op_ile,
    op_iin,
    op_inoin,
    op_Jieq,
    op_Jine,
    op_Jigt,
    op_Jilt,
    op_Jige,
    op_Jile,
    op_Jiin,
    op_Jinoin,
    op_fadd,
    op_fsub,
    op_fmul,
    op_fdiv,
    op_fcast,
    op_feq,
    op_fne,
    op_fgt,
    op_flt,
    op_fge,
    op_fle,
    op_Jfeq,
    op_Jfne,
    op_Jfgt,
    op_Jflt,
    op_Jfge,
    op_Jfle,
    op_fin,
    op_fnoin,
    op_Jfin,
    op_Jfnoin,
    op_call,
    op_scall,
    op_fcall,
    op_rcall,
    op_jmp,
    op_cast,
    op_ogetidx,
    op_osetidx,
    op_ngetidx,
    op_nsetidx,
    op_thcode_init,
    op_unused0,
    op_unused1,
    op_unused2,
    op_unused3,
    op_unused4,
    op_unused5,
    op_unused6,
    op_max
};

static void vm_exec(vm_t *vm, vm_code_t *pc)
{
}

static vm_t *vm_new(void)
{
    vm_t *vm = malloc_(sizeof(vm_t));
    int i;
    for (i = 0; i < VM_MAX_REG_SIZE; i++) {
        vm->regs_[i].o = NULL;
    }
    return vm;
}

static void vm_delete(vm_t *vm)
{
    free_(vm);
}

int main(int argc, char **argv)
{
    vm_t *vm = vm_new();
    fprintf(stderr, "sizeof(value_t)=%d\n", (int)sizeof(value_t));
    fprintf(stderr, "sizeof(vm)=%d\n", (int)sizeof(*vm));
    fprintf(stderr, "sizeof(code)=%d\n", (int)sizeof(vm_code_t));
    fprintf(stderr, "op_max=%d\n", op_max);
    //fprintf(stderr, "%lld\n", vm->r.ret.i);
    vm_exec(vm, NULL);
    vm_delete(vm);
    return 0;
}

#define _OFF_I(idx) (idx.i)
#define _OFF_X(idx) (idx.x)

#define L(l) L_op_##l : asm("##" #l);
#define _arg0(f) f()
#define _arg1(f) f(pc->a0)
#define _arg2(f) f(pc->a0, pc->a1)
#define _arg3(f) f(pc->a0, pc->a1, pc->a2)
#define _arg4(f) f(pc->a0, pc->a1, pc->a2, pc->a3)
struct knh_ObjectField_t {
    knh_hObject_t h;
    value_t *ox;
    value_t unused1;
    value_t unused2;
    value_t unused3;
};
typedef struct knh_ObjectField_t knh_ObjectField_t;
typedef void (*fvm)(vm_t *);
typedef void (*fvm2)(vm_t *, vm_code_t *);
typedef knh_Object_t *(*fcast)(knh_Object_t *o1, knh_int_t v);
typedef void (*fa1_v)(void);
typedef knh_int_t (*fa1_i)(knh_int_t);
typedef knh_float_t (*fa1_f)(knh_float_t);
typedef knh_Object_t *(*fa1_p)(knh_Object_t*);
struct knh_Method_t {
    knh_hObject_t h;
    fvm2 call;
    vm_code_t *pc;
};

#undef O
#define A(i)       (pc->a ## i)
#define NARG(idx)    (arg[idx].ival)
#define FARG(idx)    (arg[idx].fval)
#define OARG(idx)    (arg[idx].o)
#define RET(vm)    (vm->r.ret.o)
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
void __thcode_init(void **thcode)
{
    THCODE__ = thcode;
}

#define DISPATCH(pc)  goto *(THCODE[pc->code])
#define DISPATCH1(pc) ++pc;goto *(pc->addr)
#define vm_local_new(vm, n)    vm->sp += n
#define vm_local_delete(vm, n) vm->sp -= n

void run(vm_t *vm, vm_code_t *pc)
{
    value_t _r[8];
    register value_t *r = _r;
    register value_t *arg = vm->r.arg;
    static void* THCODE[] = {
        &&L_op_halt,
        &&L_op_exit,
        &&L_op_local_start,
        &&L_op_local_end,
        &&L_op_nmov,
        &&L_op_nmovx,
        &&L_op_xnmov,
        &&L_op_xnmovx,
        &&L_op_omov,
        &&L_op_omovx,
        &&L_op_xomov,
        &&L_op_xomovx,
        &&L_op_nset,
        &&L_op_xnset,
        &&L_op_oset,
        &&L_op_xoset,
        &&L_op_new,
        &&L_op_box,
        &&L_op_unbox,
        &&L_op_iadd,
        &&L_op_isub,
        &&L_op_imul,
        &&L_op_idiv,
        &&L_op_imod,
        &&L_op_icast,
        &&L_op_ieq,
        &&L_op_ine,
        &&L_op_igt,
        &&L_op_ilt,
        &&L_op_ige,
        &&L_op_ile,
        &&L_op_iin,
        &&L_op_inoin,
        &&L_op_Jieq,
        &&L_op_Jine,
        &&L_op_Jigt,
        &&L_op_Jilt,
        &&L_op_Jige,
        &&L_op_Jile,
        &&L_op_Jiin,
        &&L_op_Jinoin,
        &&L_op_fadd,
        &&L_op_fsub,
        &&L_op_fmul,
        &&L_op_fdiv,
        &&L_op_fcast,
        &&L_op_feq,
        &&L_op_fne,
        &&L_op_fgt,
        &&L_op_flt,
        &&L_op_fge,
        &&L_op_fle,
        &&L_op_Jfeq,
        &&L_op_Jfne,
        &&L_op_Jfgt,
        &&L_op_Jflt,
        &&L_op_Jfge,
        &&L_op_Jfle,
        &&L_op_fin,
        &&L_op_fnoin,
        &&L_op_Jfin,
        &&L_op_Jfnoin,
        &&L_op_call,
        &&L_op_scall,
        &&L_op_fcall,
        &&L_op_rcall_v,
        &&L_op_rcall_i,
        &&L_op_rcall_p,
        &&L_op_rcall_f,
        &&L_op_jmp,
        &&L_op_cast,
        &&L_op_ogetidx,
        &&L_op_osetidx,
        &&L_op_ngetidx,
        &&L_op_nsetidx,
        &&L_op_thcode_init,
        &&L_op_unused0,
        &&L_op_unused1,
        &&L_op_unused2,
        &&L_op_unused3,
        &&L_op_unused4,
        &&L_op_unused5,
        &&L_op_unused6,
    };
    DISPATCH(pc);
    L(halt        ) {_arg0(vmop_halt        );DISPATCH1(pc);}
    L(exit        ) {_arg0(vmop_exit        );DISPATCH1(pc);}
    L(local_start ) {_arg1(vmop_local_start );DISPATCH1(pc);}
    L(local_end   ) {_arg1(vmop_local_end   );DISPATCH1(pc);}
    L(nmov        ) {_arg2(vmop_nmov        );DISPATCH1(pc);}
    L(nmovx       ) {_arg2(vmop_nmovx       );DISPATCH1(pc);}
    L(xnmov       ) {_arg2(vmop_xnmov       );DISPATCH1(pc);}
    L(xnmovx      ) {_arg2(vmop_xnmovx      );DISPATCH1(pc);}
    L(omov        ) {_arg2(vmop_omov        );DISPATCH1(pc);}
    L(omovx       ) {_arg2(vmop_omovx       );DISPATCH1(pc);}
    L(xomov       ) {_arg2(vmop_xomov       );DISPATCH1(pc);}
    L(xomovx      ) {_arg2(vmop_xomovx      );DISPATCH1(pc);}
    L(nset        ) {_arg2(vmop_nset        );DISPATCH1(pc);}
    L(xnset       ) {_arg2(vmop_xnset       );DISPATCH1(pc);}
    L(oset        ) {_arg2(vmop_oset        );DISPATCH1(pc);}
    L(xoset       ) {_arg2(vmop_xoset       );DISPATCH1(pc);}
    L(new         ) {_arg2(vmop_new         );DISPATCH1(pc);}
    L(box         ) {_arg2(vmop_box         );DISPATCH1(pc);}
    L(unbox       ) {_arg2(vmop_unbox       );DISPATCH1(pc);}
    L(iadd        ) {_arg3(vmop_iadd        );DISPATCH1(pc);}
    L(isub        ) {_arg3(vmop_isub        );DISPATCH1(pc);}
    L(imul        ) {_arg3(vmop_imul        );DISPATCH1(pc);}
    L(idiv        ) {_arg3(vmop_idiv        );DISPATCH1(pc);}
    L(imod        ) {_arg3(vmop_imod        );DISPATCH1(pc);}
    L(icast       ) {_arg2(vmop_icast       );DISPATCH1(pc);}
    L(ieq         ) {_arg3(vmop_ieq         );DISPATCH1(pc);}
    L(ine         ) {_arg3(vmop_ine         );DISPATCH1(pc);}
    L(igt         ) {_arg3(vmop_igt         );DISPATCH1(pc);}
    L(ilt         ) {_arg3(vmop_ilt         );DISPATCH1(pc);}
    L(ige         ) {_arg3(vmop_ige         );DISPATCH1(pc);}
    L(ile         ) {_arg3(vmop_ile         );DISPATCH1(pc);}
    L(iin         ) {_arg4(vmop_iin         );DISPATCH1(pc);}
    L(inoin       ) {_arg4(vmop_inoin       );DISPATCH1(pc);}
    L(Jieq        ) {_arg3(vmop_Jieq        );DISPATCH1(pc);}
    L(Jine        ) {_arg3(vmop_Jine        );DISPATCH1(pc);}
    L(Jigt        ) {_arg3(vmop_Jigt        );DISPATCH1(pc);}
    L(Jilt        ) {_arg3(vmop_Jilt        );DISPATCH1(pc);}
    L(Jige        ) {_arg3(vmop_Jige        );DISPATCH1(pc);}
    L(Jile        ) {_arg3(vmop_Jile        );DISPATCH1(pc);}
    L(Jiin        ) {_arg4(vmop_Jiin        );DISPATCH1(pc);}
    L(Jinoin      ) {_arg4(vmop_Jinoin      );DISPATCH1(pc);}
    L(fadd        ) {_arg3(vmop_fadd        );DISPATCH1(pc);}
    L(fsub        ) {_arg3(vmop_fsub        );DISPATCH1(pc);}
    L(fmul        ) {_arg3(vmop_fmul        );DISPATCH1(pc);}
    L(fdiv        ) {_arg3(vmop_fdiv        );DISPATCH1(pc);}
    L(fcast       ) {_arg2(vmop_fcast       );DISPATCH1(pc);}
    L(feq         ) {_arg3(vmop_feq         );DISPATCH1(pc);}
    L(fne         ) {_arg3(vmop_fne         );DISPATCH1(pc);}
    L(fgt         ) {_arg3(vmop_fgt         );DISPATCH1(pc);}
    L(flt         ) {_arg3(vmop_flt         );DISPATCH1(pc);}
    L(fge         ) {_arg3(vmop_fge         );DISPATCH1(pc);}
    L(fle         ) {_arg3(vmop_fle         );DISPATCH1(pc);}
    L(fin         ) {_arg4(vmop_fin         );DISPATCH1(pc);}
    L(fnoin       ) {_arg4(vmop_fnoin       );DISPATCH1(pc);}
    L(Jfeq        ) {_arg3(vmop_Jfeq        );DISPATCH1(pc);}
    L(Jfne        ) {_arg3(vmop_Jfne        );DISPATCH1(pc);}
    L(Jfgt        ) {_arg3(vmop_Jfgt        );DISPATCH1(pc);}
    L(Jflt        ) {_arg3(vmop_Jflt        );DISPATCH1(pc);}
    L(Jfge        ) {_arg3(vmop_Jfge        );DISPATCH1(pc);}
    L(Jfle        ) {_arg3(vmop_Jfle        );DISPATCH1(pc);}
    L(Jfin        ) {_arg4(vmop_Jfin        );DISPATCH1(pc);}
    L(Jfnoin      ) {_arg4(vmop_Jfnoin      );DISPATCH1(pc);}
    L(call        ) {_arg3(vmop_call        );DISPATCH1(pc);}
    L(scall       ) {_arg3(vmop_scall       );DISPATCH1(pc);}
    L(fcall       ) {_arg3(vmop_fcall       );DISPATCH1(pc);}
    L(rcall_v     ) {_arg3(vmop_rcall_v     );DISPATCH1(pc);}
    L(rcall_i     ) {_arg3(vmop_rcall_i     );DISPATCH1(pc);}
    L(rcall_p     ) {_arg3(vmop_rcall_p     );DISPATCH1(pc);}
    L(rcall_f     ) {_arg3(vmop_rcall_f     );DISPATCH1(pc);}
    L(jmp         ) {_arg1(vmop_jmp         );DISPATCH1(pc);}
    L(cast        ) {_arg2(vmop_cast        );DISPATCH1(pc);}
    L(ogetidx     ) {_arg3(vmop_ogetidx     );DISPATCH1(pc);}
    L(osetidx     ) {_arg3(vmop_osetidx     );DISPATCH1(pc);}
    L(ngetidx     ) {_arg3(vmop_ngetidx     );DISPATCH1(pc);}
    L(nsetidx     ) {_arg3(vmop_nsetidx     );DISPATCH1(pc);}
    L(thcode_init ) {_arg0(vmop_thcode_init );DISPATCH1(pc);}
    L(unused0     ) {_arg1(vmop_unused0     );DISPATCH1(pc);}
    L(unused1     ) {_arg1(vmop_unused1     );DISPATCH1(pc);}
    L(unused2     ) {_arg1(vmop_unused2     );DISPATCH1(pc);}
    L(unused3     ) {_arg1(vmop_unused3     );DISPATCH1(pc);}
    L(unused4     ) {_arg1(vmop_unused4     );DISPATCH1(pc);}
    L(unused5     ) {_arg1(vmop_unused5     );DISPATCH1(pc);}
    L(unused6     ) {_arg1(vmop_unused6     );DISPATCH1(pc);}
}
