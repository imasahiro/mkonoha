#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "konoha.h"
#include "memory.h"
#include "vmop.h"
#include "vm.h"

typedef knh_value_t value_t;
typedef struct vm vm_t;
typedef struct vm_code vm_code_t;

typedef struct knh_ObjectField_t knh_ObjectField_t;
DEF_ARRAY_S_T(data);
DEF_ARRAY_S_STRUCT(data);
DEF_ARRAY_S_OP(data);

DEF_ARRAY_STRUCT(Object);
DEF_ARRAY_OP(Object);

struct vm_code {
    union {
        intptr_t code;
        void *addr;
    } c;
    value_t a0;
    value_t a1;
    value_t a2;
    value_t a3;
};

#define __(n) {(knh_int_t)n}
typedef void (*fa1_v)(void);
typedef knh_int_t     (*fa1_i)(knh_int_t);
typedef knh_float_t   (*fa1_f)(knh_float_t);
typedef knh_Object_t *(*fa1_p)(knh_Object_t*);

static bool vm_code_isInited(void);
static void exit_with_msg(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

struct knh_Method_t *new_Method(fvm2 func, vm_code_t *pc);
static inline void *__new(size_t size, knh_class_t cid);

struct knh_Method_t *new_Method(fvm2 func, vm_code_t *pc)
{
    struct knh_Method_t *mtd = new_(Method);
    mtd->call = func;
    mtd->pc   = pc;
    return mtd;
}

static vm_t *vm_new(void)
{
    vm_t *vm = cast(vm_t*,malloc_(sizeof(vm_t)));
    int i;
    for (i = 0; i < VM_MAX_REG_SIZE; i++) {
        vm->regs_[i].o = NULL;
    }
    vm->r.ret.dval = 0;
    vm->sp = malloc_(sizeof(void*) * 512);
    return vm;
}

static void vm_delete(vm_t *vm)
{
    free_(vm);
}

static void dump(knh_value_t *sp)
{
    int i;
    for (i = 0; i < 5; i++) {
        fprintf(stderr, "sp%d=%lld,", i, sp[i].ival);
    }
    fprintf(stderr, "\n");
}
static void VM_DBG_P(const char *f, vm_code_t *pc, vm_t *vm, value_t *sp)
{
#ifdef DBG_VM
    fprintf(stderr, "%18s pc=%p addr=%p, %8lld %8lld %8lld\n", f, pc, pc->c.addr, pc->a0.ival, pc->a1.ival, pc->a2.ival);
    //fprintf(stderr, "next=%p, %p\n", pc+1, (pc+1)->c.addr);
    //asm volatile("int3");
    //fprintf(stderr, "lsp::(%p)", sp);
    //dump(sp);
    //fprintf(stderr, "vsp::(%p)", vm->sp);
    //dump(vm->sp);
#endif
}
static void VM_DBG_N(const char *f, vm_code_t *pc)
{
#if 0
    fprintf(stderr, "%s n pc=%p\n", f, pc);
#endif
}

#define _OFF_I(idx) (idx.i)
#define _OFF_X(idx) (idx.x)
#define L(l)    L_op_##l : asm("##" #l);
#define __a0(f) f()
#define __a1(f) f(pc->a0)
#define __a2(f) f(pc->a0, pc->a1)
#define __a3(f) f(pc->a0, pc->a1, pc->a2)
#define __a4(f) f(pc->a0, pc->a1, pc->a2, pc->a3)
#define _arg0(f) VM_DBG_P(#f, pc, vm, sp);__a0(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg1(f) VM_DBG_P(#f, pc, vm, sp);__a1(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg2(f) VM_DBG_P(#f, pc, vm, sp);__a2(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg3(f) VM_DBG_P(#f, pc, vm, sp);__a3(f); VM_DBG_N(#f, pc); DISPATCH1(pc)
#define _arg4(f) VM_DBG_P(#f, pc, vm, sp);__a4(f); VM_DBG_N(#f, pc); DISPATCH1(pc)

#undef O
#define A(i)         (pc->a ## i)
#define NARG(idx)    (arg[idx].ival)
#define FARG(idx)    (arg[idx].fval)
#define OARG(idx)    (arg[idx].o)
#define RET(vm)    (vm->r.ret.o)
#define RETv(vm)   (vm->r.ret.dval)
#define LOCAL(idx)  ((sp[A(idx).ival]).dval)
#define V(idx)     ((r[A(idx).ival]).dval)
#define N(idx)     ((r[A(idx).ival]).ival)
#define O(idx)     ((r[A(idx).ival]).o)
#define NX(idx)    (r[_OFF_I((A(idx).index))].ox->ox[_OFF_X((A(idx).index))].ival)
#define OX(idx)    (r[_OFF_I((A(idx).index))].ox->ox[_OFF_X((A(idx).index))].o)
#define F(idx)     ((r[A(idx).ival]).fval)
#define P(idx)     ((r[A(idx).ival]).o)
#define MTD(idx)   (r[A(idx).ival]).mtd
#define FUNC(idx)  ((fvm2)(r[A(idx).ival]).o)
#define FCAST(idx) ((fcast_)(r[A(idx).ival]).o)
#define NC(idx)    (A(idx).ival)
#define FC(idx)    (A(idx).fval)
#define OC(idx)    (A(idx).o)
#define C(idx)     ((knh_class_t)A(idx).ival)
#define NA(idx)    ((Array(data)*)  r[A(idx).ival].o)
#define OA(idx)    ((Array(Object)*) r[A(idx).ival].o)

#if (defined(__linux__) && (defined(__i386__) || defined(__x86_64__)) && (defined(__GNUC__) && __GNUC__ >= 3))
#define K_USING_VMASMDISPATCH 1
#endif
#ifdef K_USING_VMASMDISPATCH
#define jmp(add)   \
    asm volatile("jmp *%0;": : "g"(add));\
    goto *(add);
#else
#define jmp(add)   goto *(add)
#endif

#define __JMP__()  {\
    vm_code_t *p = (vm_code_t *) OC(0);\
    pc = p;\
    jmp(p->c.addr);\
}
#define DISPATCH(pc)  goto *(THCODE[pc->c.code])
#define DISPATCH1(pc) {\
    void *p_;\
    pc = pc+1;\
    p_ = pc->c.addr;\
    jmp(p_);\
}
#define DISPATCH2(pc)  jmp(pc->c.addr)
//#define vm_local_new(vm, n)    vm->sp = (void*)((intptr_t)vm->sp + n);
//#define vm_local_delete(vm, n) vm->sp = (void*)((intptr_t)vm->sp - n);

static value_t *vm_local_new(vm_t *vm, int n)
{
    value_t *sp = vm->sp;
    vm->sp = (void*)((intptr_t)vm->sp + n*sizeof(void*));
    return sp;
}
static value_t *vm_local_delete(vm_t *vm, int n)
{
    value_t *sp = vm->sp;
    vm->sp = (void*)((intptr_t)vm->sp - n*sizeof(void*));
    return sp;
}

static knh_Object_t *new_Object_(knh_class_t cid)
{
    return NULL;
}
static knh_Object_t *new_box(knh_class_t cid, knh_int_t i)
{
    return NULL;
}
static knh_int_t new_unbox(knh_class_t cid, knh_Object_t *o)
{
    return 0;
}

static void _halt(void)
{
    //fprintf(stderr, "halt!!\n");
    //exit(EXIT_FAILURE);
    asm volatile("int3");
}

static void **THCODE__ = NULL;
static bool thcode_inited = false;
static bool vm_code_isInited(void)
{
    return thcode_inited && THCODE__ != NULL;
}
static inline bool isOpCall(vm_code_t *pc)
{
    if (vm_code_isInited()) {
        return THCODE__[op_call] == pc->c.addr;
    }
    return false;
}
static inline void SetOpBCall(vm_code_t *pc)
{
    if (vm_code_isInited()) {
        pc->c.addr = THCODE__[op_bcall];
    }
}
static inline bool hasJump(vm_code_t *pc)
{
    enum opcode op = pc->c.code;
    return ((op_Jieq <= op && op <= op_Jinoin) ||
            (op_Jfeq <= op && op <= op_Jfnoin) ||
            (op_JieqC <= op && op <= op_JileC) ||
            (op_JfeqC <= op && op <= op_JfleC));
}
static void __thcode_init__(vm_code_t *pc, void **thcode)
{
    if (!thcode_inited) {
        THCODE__ = thcode;
        thcode_inited = true;
        return;
    }
}
static vm_code_t *thcode_init(vm_code_t *pc)
{
    vm_code_t *base = pc;
    void **thcode =  THCODE__;
    while (pc->c.code != op_exit) {
        if (hasJump(pc)) {
            //asm volatile("int3");
            //fprintf(stderr, "%p\n", pc->a0.ptr);
            vm_code_t *tmp;
            for (tmp = base; tmp->c.code != op_exit; tmp++) {
                //fprintf(stderr, "%p %p %p\n", tmp, pc, pc->a0.ptr);
                if (tmp == pc->a0.ptr) {
                    vm_code_t *a_ = cast(vm_code_t*, pc->a0.ptr);
                    pc->a0.ptr = a_;
                }
            }
        }
        pc->c.addr = thcode[pc->c.code];
        pc++;
    }
    pc->c.addr = thcode[pc->c.code];
    base = base - 1;
    base->c.code = op_enter;
    //base->a0.ival = (base+1)->a0.ival;
    //base->a1.ival = (base+1)->a1.ival;
    //base->a2.ival = (base+1)->a2.ival;
    //base->a3.ival = (base+1)->a3.ival;
    return base;
}
static vm_code_t *vm_code_init(vm_t *vm, vm_code_t *code)
{
    if (!vm_code_isInited()) {
        vm_code_t __code = {__(op_thcode),      __(0), __(0), __(0), __(0)};
        vm_exec(vm, &__code);
        if (!vm_code_isInited()) {
            exit_with_msg("threaded code init failed");
        }
    }
    //intptr_t opstart = code[0].c.code;
    code = thcode_init(code);
    //code[0].c.code = opstart;
    return code;
}
static inline void push_addr(vm_t *vm, void **l)
{
    register value_t *sp = vm->sp;
    sp[0].ptr = l;
    vm->sp = sp + 1;
}
static inline void push_addr2(vm_t *vm, void **l1, void **l2)
{
    register value_t *sp = vm->sp;
    sp[0].ptr = l1;
    sp[1].ptr = l2;
    vm->sp = sp + 2;
}
static inline void push_addr3(vm_t *vm,void **l1, void **l2, void **l3)
{
    register value_t *sp = vm->sp;
    sp[0].ptr = l1;
    sp[1].ptr = l2;
    sp[2].ptr = l3;
    vm->sp = sp + 3;
}
static inline void **pop_addr(vm_t *vm)
{
    register value_t *sp = vm->sp - 1;
    vm->sp = sp;
    return sp[0].ptr;
}
static void vm_exec(vm_t *vm, vm_code_t *pc_)
{
    register value_t *r =   vm->r.reg;
    register value_t *arg = vm->r.arg;
    register value_t *sp asm("r14");
    register vm_code_t *pc asm("r15") = pc_;
    static void* THCODE[] = {
        &&L_op_halt,
        &&L_op_enter,
        &&L_op_exit,
        &&L_op_local_start,
        &&L_op_local_end,
        &&L_op_movlr,
        &&L_op_movrl,
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
        &&L_op_ipow,
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
        &&L_op_fpow,
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
        &&L_op_bcall,
        &&L_op_scall,
        &&L_op_fcall,
        &&L_op_ncall_v,
        &&L_op_ncall_i,
        &&L_op_ncall_p,
        &&L_op_ncall_f,
        &&L_op_ret,
        &&L_op_retv,
        &&L_op_jmp,
        &&L_op_cast,
        &&L_op_ogetidx,
        &&L_op_osetidx,
        &&L_op_ngetidx,
        &&L_op_nsetidx,
        /* opt */
        &&L_op_iaddC,
        &&L_op_isubC,
        &&L_op_imulC,
        &&L_op_idivC,
        &&L_op_imodC,
        &&L_op_ieqC,
        &&L_op_ineC,
        &&L_op_igtC,
        &&L_op_iltC,
        &&L_op_igeC,
        &&L_op_ileC,
        &&L_op_JieqC,
        &&L_op_JineC,
        &&L_op_JigtC,
        &&L_op_JiltC,
        &&L_op_JigeC,
        &&L_op_JileC,
        &&L_op_faddC,
        &&L_op_fsubC,
        &&L_op_fmulC,
        &&L_op_fdivC,
        &&L_op_feqC,
        &&L_op_fneC,
        &&L_op_fgtC,
        &&L_op_fltC,
        &&L_op_fgeC,
        &&L_op_fleC,
        &&L_op_JfeqC,
        &&L_op_JfneC,
        &&L_op_JfgtC,
        &&L_op_JfltC,
        &&L_op_JfgeC,
        &&L_op_JfleC,
        /* unused */

        &&L_op_thcode,
        &&L_op_unused0,
        &&L_op_unused1,
        &&L_op_unused2,
        &&L_op_unused3,
        &&L_op_unused4,
        &&L_op_unused5,
        &&L_op_unused6,
        &&L_op_unused7,
    };

    push_addr(vm, &&L_return);
    sp = vm->sp;
    DISPATCH(pc);
    L(halt        ) {_arg0(vmop_halt        );}
    L(enter       ) {_arg0(vmop_enter       );}
    L(exit        ) {_arg0(vmop_exit        );}
    L(local_start ) {_arg1(vmop_local_start );}
    L(local_end   ) {_arg1(vmop_local_end   );}
    L(movlr       ) {_arg2(vmop_movlr       );}
    L(movrl       ) {_arg2(vmop_movrl       );}
    L(nmov        ) {_arg2(vmop_nmov        );}
    L(nmovx       ) {_arg2(vmop_nmovx       );}
    L(xnmov       ) {_arg2(vmop_xnmov       );}
    L(xnmovx      ) {_arg2(vmop_xnmovx      );}
    L(omov        ) {_arg2(vmop_omov        );}
    L(omovx       ) {_arg2(vmop_omovx       );}
    L(xomov       ) {_arg2(vmop_xomov       );}
    L(xomovx      ) {_arg2(vmop_xomovx      );}
    L(nset        ) {_arg2(vmop_nset        );}
    L(xnset       ) {_arg2(vmop_xnset       );}
    L(oset        ) {_arg2(vmop_oset        );}
    L(xoset       ) {_arg2(vmop_xoset       );}
    L(new         ) {_arg2(vmop_new         );}
    L(box         ) {_arg2(vmop_box         );}
    L(unbox       ) {_arg2(vmop_unbox       );}
    L(iadd        ) {_arg3(vmop_iadd        );}
    L(isub        ) {_arg3(vmop_isub        );}
    L(imul        ) {_arg3(vmop_imul        );}
    L(idiv        ) {_arg3(vmop_idiv        );}
    L(imod        ) {_arg3(vmop_imod        );}
    L(ipow        ) {_arg2(vmop_ipow        );}
    L(icast       ) {_arg2(vmop_icast       );}
    L(ieq         ) {_arg3(vmop_ieq         );}
    L(ine         ) {_arg3(vmop_ine         );}
    L(igt         ) {_arg3(vmop_igt         );}
    L(ilt         ) {_arg3(vmop_ilt         );}
    L(ige         ) {_arg3(vmop_ige         );}
    L(ile         ) {_arg3(vmop_ile         );}
    L(iin         ) {_arg4(vmop_iin         );}
    L(inoin       ) {_arg4(vmop_inoin       );}
    L(Jieq        ) {_arg3(vmop_Jieq        );}
    L(Jine        ) {_arg3(vmop_Jine        );}
    L(Jigt        ) {_arg3(vmop_Jigt        );}
    L(Jilt        ) {_arg3(vmop_Jilt        );}
    L(Jige        ) {_arg3(vmop_Jige        );}
    L(Jile        ) {_arg3(vmop_Jile        );}
    L(Jiin        ) {_arg4(vmop_Jiin        );}
    L(Jinoin      ) {_arg4(vmop_Jinoin      );}
    L(fadd        ) {_arg3(vmop_fadd        );}
    L(fsub        ) {_arg3(vmop_fsub        );}
    L(fmul        ) {_arg3(vmop_fmul        );}
    L(fdiv        ) {_arg3(vmop_fdiv        );}
    L(fpow        ) {_arg2(vmop_fpow        );}
    L(fcast       ) {_arg2(vmop_fcast       );}
    L(feq         ) {_arg3(vmop_feq         );}
    L(fne         ) {_arg3(vmop_fne         );}
    L(fgt         ) {_arg3(vmop_fgt         );}
    L(flt         ) {_arg3(vmop_flt         );}
    L(fge         ) {_arg3(vmop_fge         );}
    L(fle         ) {_arg3(vmop_fle         );}
    L(fin         ) {_arg4(vmop_fin         );}
    L(fnoin       ) {_arg4(vmop_fnoin       );}
    L(Jfeq        ) {_arg3(vmop_Jfeq        );}
    L(Jfne        ) {_arg3(vmop_Jfne        );}
    L(Jfgt        ) {_arg3(vmop_Jfgt        );}
    L(Jflt        ) {_arg3(vmop_Jflt        );}
    L(Jfge        ) {_arg3(vmop_Jfge        );}
    L(Jfle        ) {_arg3(vmop_Jfle        );}
    L(Jfin        ) {_arg4(vmop_Jfin        );}
    L(Jfnoin      ) {_arg4(vmop_Jfnoin      );}
    L(call        ) {_arg3(vmop_call        );}
    L(bcall       ) {_arg3(vmop_bcall       );}
    L(scall       ) {_arg3(vmop_scall       );}
    L(fcall       ) {_arg3(vmop_fcall       );}
    L(ncall_v     ) {_arg3(vmop_ncall_v     );}
    L(ncall_i     ) {_arg3(vmop_ncall_i     );}
    L(ncall_p     ) {_arg3(vmop_ncall_p     );}
    L(ncall_f     ) {_arg3(vmop_ncall_f     );}
    L(ret         ) {_arg1(vmop_ret         );}
    L(retv        ) {_arg1(vmop_retv        );}
    L(jmp         ) {_arg1(vmop_jmp         );}
    L(cast        ) {_arg2(vmop_cast        );}
    L(ogetidx     ) {_arg3(vmop_ogetidx     );}
    L(osetidx     ) {_arg3(vmop_osetidx     );}
    L(ngetidx     ) {_arg3(vmop_ngetidx     );}
    L(nsetidx     ) {_arg3(vmop_nsetidx     );}
    /* opt */
    L(iaddC       ) {_arg3(vmop_iaddC       );}
    L(isubC       ) {_arg3(vmop_isubC       );}
    L(imulC       ) {_arg3(vmop_imulC       );}
    L(idivC       ) {_arg3(vmop_idivC       );}
    L(imodC       ) {_arg3(vmop_imodC       );}
    L(ieqC        ) {_arg3(vmop_ieqC        );}
    L(ineC        ) {_arg3(vmop_ineC        );}
    L(igtC        ) {_arg3(vmop_igtC        );}
    L(iltC        ) {_arg3(vmop_iltC        );}
    L(igeC        ) {_arg3(vmop_igeC        );}
    L(ileC        ) {_arg3(vmop_ileC        );}
    L(JieqC       ) {_arg3(vmop_JieqC       );}
    L(JineC       ) {_arg3(vmop_JineC       );}
    L(JigtC       ) {_arg3(vmop_JigtC       );}
    L(JiltC       ) {_arg3(vmop_JiltC       );}
    L(JigeC       ) {_arg3(vmop_JigeC       );}
    L(JileC       ) {_arg3(vmop_JileC       );}
    L(faddC       ) {_arg3(vmop_faddC       );}
    L(fsubC       ) {_arg3(vmop_fsubC       );}
    L(fmulC       ) {_arg3(vmop_fmulC       );}
    L(fdivC       ) {_arg3(vmop_fdivC       );}
    L(feqC        ) {_arg3(vmop_feqC        );}
    L(fneC        ) {_arg3(vmop_fneC        );}
    L(fgtC        ) {_arg3(vmop_fgtC        );}
    L(fltC        ) {_arg3(vmop_fltC        );}
    L(fgeC        ) {_arg3(vmop_fgeC        );}
    L(fleC        ) {_arg3(vmop_fleC        );}
    L(JfeqC       ) {_arg3(vmop_JfeqC       );}
    L(JfneC       ) {_arg3(vmop_JfneC       );}
    L(JfgtC       ) {_arg3(vmop_JfgtC       );}
    L(JfltC       ) {_arg3(vmop_JfltC       );}
    L(JfgeC       ) {_arg3(vmop_JfgeC       );}
    L(JfleC       ) {_arg3(vmop_JfleC       );}
    /* unused */
    L(thcode      ) {_arg0(vmop_thcode      );}
    L(unused0     ) {_arg1(vmop_unused0     );}
    L(unused1     ) {_arg1(vmop_unused1     );}
    L(unused2     ) {_arg1(vmop_unused2     );}
    L(unused3     ) {_arg1(vmop_unused3     );}
    L(unused4     ) {_arg1(vmop_unused4     );}
    L(unused5     ) {_arg1(vmop_unused5     );}
    L(unused6     ) {_arg1(vmop_unused6     );}
    L(unused7     ) {_arg1(vmop_unused7     );}
L_return:
    return;
}

#include "./vmbuilder.c"
