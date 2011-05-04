enum opcode {
    op_halt,
    op_enter,
    op_exit,
    op_local_start,
    op_local_end,
    op_movlr,
    op_movrl,
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
    op_ipow,
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
    op_fpow,
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
    op_bcall,
    op_scall,
    op_fcall,
    op_ncall_v,
    op_ncall_i,
    op_ncall_p,
    op_ncall_f,
    op_ret,
    op_retv,
    op_jmp,
    op_cast,
    op_ogetidx,
    op_osetidx,
    op_ngetidx,
    op_nsetidx,
    /* opt */
    op_iaddC,
    op_isubC,
    op_imulC,
    op_idivC,
    op_imodC,
    op_ieqC,
    op_ineC,
    op_igtC,
    op_iltC,
    op_igeC,
    op_ileC,
    op_JieqC,
    op_JineC,
    op_JigtC,
    op_JiltC,
    op_JigeC,
    op_JileC,
    op_faddC,
    op_fsubC,
    op_fmulC,
    op_fdivC,
    op_feqC,
    op_fneC,
    op_fgtC,
    op_fltC,
    op_fgeC,
    op_fleC,
    op_JfeqC,
    op_JfneC,
    op_JfgtC,
    op_JfltC,
    op_JfgeC,
    op_JfleC,
    /* unused */
    op_thcode,
    op_unused0,
    op_unused1,
    op_unused2,
    op_unused3,
    op_unused4,
    op_unused5,
    op_unused6,
    op_unused7,
    op_max
};


#define vmop_halt()                          _halt()
#define vmop_enter()                         
#define vmop_exit()                          return
#define vmop_local_start(i0)                 sp = vm_local_new(vm, NC(0))
#define vmop_local_end(i0)                   sp = vm_local_delete(vm, NC(0))
#define vmop_movlr(i0, i1)                   LOCAL (0) = N (1)
#define vmop_movrl(i0, i1)                   N (0) = LOCAL (1)
#define vmop_nmov(i0, i1)                    N (0) = N (1)
#define vmop_nmovx(i0, i1)                   N (0) = NX(1)
#define vmop_xnmov(i0, i1)                   NX(0) = N (1)
#define vmop_xnmovx(i0, i1)                  NX(0) = NX(1)
#define vmop_omov(i0, i1)                    O (0) = O (1)
#define vmop_omovx(i0, i1)                   O (0) = OX(1)
#define vmop_xomov(i0, i1)                   OX(0) = O (1)
#define vmop_xomovx(i0, i1)                  OX(0) = OX(1)
#define vmop_nset(i0, i1)                    N (0) = NC(1)
#define vmop_xnset(i0, i1)                   NX(0) = NC(1)
#define vmop_oset(i0, i1)                    O (0) = OC(1)
#define vmop_xoset(i0, i1)                   OX(0) = OC(1)
#define vmop_new(i0, i1)                     O (0) = new_Object_(C(1))
#define vmop_box(i0, i1)                     O (0) = new_box(C(1), N (2))
#define vmop_unbox(i0, i1)                   N (0) = new_unbox(C(1), O (2))
#define vmop_iadd(i0, i1, i2)                N (0) = N(1) + N(2)
#define vmop_isub(i0, i1, i2)                N (0) = N(1) - N(2)
#define vmop_imul(i0, i1, i2)                N (0) = N(1) * N(2)
#define vmop_idiv(i0, i1, i2)                N (0) = N(1) / N(2)
#define vmop_imod(i0, i1, i2)                N (0) = N(1) % N(2)
#define vmop_ipow(i0, i1)                    N (0) = N(1) * N(1)
#define vmop_icast(i0, i1)                   N (0) = (knh_int_t) F(1)
#define vmop_ieq(i0, i1, i2)                 N (0) = N(1) == N(2)
#define vmop_ine(i0, i1, i2)                 N (0) = N(1) != N(2)
#define vmop_igt(i0, i1, i2)                 N (0) = N(1) >  N(2)
#define vmop_ilt(i0, i1, i2)                 N (0) = N(1) <  N(2)
#define vmop_ige(i0, i1, i2)                 N (0) = N(1) >= N(2)
#define vmop_ile(i0, i1, i2)                 N (0) = N(1) <= N(2)
#define vmop_iin(i0, i1, i2, i3)             N (0) = N(2) <= N(1) && N(1) <= N(3)
#define vmop_inoin(i0, i1, i2, i3)           N (0) = N(1) <= N(2) && N(3) <= N(1)
#define vmop_Jieq(i0, i1, i2)                if (!(N(1) == N(2))) __JMP__()
#define vmop_Jine(i0, i1, i2)                if (!(N(1) != N(2))) __JMP__()
#define vmop_Jigt(i0, i1, i2)                if (!(N(1) >  N(2))) __JMP__()
#define vmop_Jilt(i0, i1, i2)                if (!(N(1) <  N(2))) __JMP__()
#define vmop_Jige(i0, i1, i2)                if (!(N(1) >= N(2))) __JMP__()
#define vmop_Jile(i0, i1, i2)                if (!(N(1) <= N(2))) __JMP__()
#define vmop_Jiin(i0, i1, i2, i3)            if (!(N(2) <= N(1) && N(1) <= N(3))) __JMP__();
#define vmop_Jinoin(i0, i1, i2, i3)          if (!(N(1) <= N(2) && N(3) <= N(1))) __JMP__();
#define vmop_fadd(i0, i1, i2)                F (0) = F(1) + F(2)
#define vmop_fsub(i0, i1, i2)                F (0) = F(1) - F(2)
#define vmop_fmul(i0, i1, i2)                F (0) = F(1) * F(2)
#define vmop_fdiv(i0, i1, i2)                F (0) = F(1) / F(2)
#define vmop_fpow(i0, i1)                    F (0) = F(1) * F(1)
#define vmop_fcast(i0, i1)                   F (0) = (knh_float_t)N(1)
#define vmop_feq(i0, i1, i2)                 F (0) = F(1) == F(2)
#define vmop_fne(i0, i1, i2)                 F (0) = F(1) != F(2)
#define vmop_fgt(i0, i1, i2)                 F (0) = F(1) >  F(2)
#define vmop_flt(i0, i1, i2)                 F (0) = F(1) <  F(2)
#define vmop_fge(i0, i1, i2)                 F (0) = F(1) >= F(2)
#define vmop_fle(i0, i1, i2)                 F (0) = F(1) <= F(2)
#define vmop_fin(i0, i1, i2, i3)             F (0) = F(2) <= F(1) && F(1) <= F(3)
#define vmop_fnoin(i0, i1, i2, i3)           F (0) = F(1) <= F(2) && F(3) <= F(1)
#define vmop_Jfeq(i0, i1, i2)                if (!(N(1) == N(2))) __JMP__();
#define vmop_Jfne(i0, i1, i2)                if (!(N(1) != N(2))) __JMP__();
#define vmop_Jfgt(i0, i1, i2)                if (!(N(1) >  N(2))) __JMP__();
#define vmop_Jflt(i0, i1, i2)                if (!(N(1) <  N(2))) __JMP__();
#define vmop_Jfge(i0, i1, i2)                if (!(N(1) >= N(2))) __JMP__();
#define vmop_Jfle(i0, i1, i2)                if (!(N(1) <= N(2))) __JMP__();
#define vmop_Jfin(i0, i1, i2, i3)            if (!(N(2) <= N(1) && N(1) <= N(3))) __JMP__();
#define vmop_Jfnoin(i0, i1, i2, i3)          if (!(N(1) <= N(2) && N(3) <= N(1))) __JMP__();
#define vmop_call(i0, i1, i2)                MTD(1)->call(vm, MTD(1)->pc);  P(0) = RET(vm)
#define vmop_bcall(i0, i1, i2) {\
    push_addr3(vm, (void**)sp,(void**)pc, &&L_call_after);\
    pc = (vm_code_t*)OC(2);     \
    DISPATCH2(pc);   \
    L_call_after:;  \
    pc = (vm_code_t*) pop_addr(vm);\
    sp = (value_t*) pop_addr(vm);\
    V(0) = RETv(vm);\
    DISPATCH1(pc);\
}
#define vmop_scall(i0, i1, i2)               ((fvm2)P(2)) (vm, MTD(1)->pc); P(0) = RET(vm)
#define vmop_fcall(i0, i1, i2)               ((fvm)OC(2)) (vm); P(0) = RET(vm)
#define vmop_ncall_v(i0, i1, i2)             ((fa1_v)OC(2)) ()
#define vmop_ncall_i(i0, i1, i2)             N(0) = ((fa1_i)OC(2)) (NARG(0))
#define vmop_ncall_p(i0, i1, i2)             P(0) = ((fa1_p)OC(2)) (OARG(0))
#define vmop_ncall_f(i0, i1, i2)             F(0) = ((fa1_f)OC(2)) (FARG(0))
#define __vmop_ret(v0)                       {\
    RETv(vm) = v0;\
    jmp(pop_addr(vm));\
}
#define vmop_ret(i0)                         __vmop_ret(V(0))
#define vmop_retv(i0)                        __vmop_ret(NC(0))
#define vmop_jmp(i0)                         jmp(OC(0))
#define vmop_cast(i0, i1)                    O (0) = FCAST(0) (P(1), N(1))
#define vmop_ngetidx(i0, i1, i2)             V (0) = Array_get(data,  NA(1), N(2))
#define vmop_ogetidx(i0, i1, i2)             O (0) = Array_get(Object, OA(1), N(2))
#define vmop_nsetidx(i0, i1, i2)             Array_set(data,  NA(1), N(2), V(0))
#define vmop_osetidx(i0, i1, i2)             Array_set(Object, OA(1), N(2), O(0))
#define vmop_thcode()                        __thcode_init__(pc, THCODE); jmp(pop_addr(vm))
#define vmop_unused0(i0)                     FUNC(0) (vm, pc)
#define vmop_unused1(i0)                     FUNC(0) (vm, pc)
#define vmop_unused2(i0)                     FUNC(0) (vm, pc)
#define vmop_unused3(i0)                     FUNC(0) (vm, pc)
#define vmop_unused4(i0)                     FUNC(0) (vm, pc)
#define vmop_unused5(i0)                     FUNC(0) (vm, pc)
#define vmop_unused6(i0)                     FUNC(0) (vm, pc)
#define vmop_unused7(i0)                     FUNC(0) (vm, pc)

/* opt */
#define vmop_iaddC(i0, i1, i2)                N (0) = N(1) + NC(2)
#define vmop_isubC(i0, i1, i2)                N (0) = N(1) - NC(2)
#define vmop_imulC(i0, i1, i2)                N (0) = N(1) * NC(2)
#define vmop_idivC(i0, i1, i2)                N (0) = N(1) / NC(2)
#define vmop_imodC(i0, i1, i2)                N (0) = N(1) % NC(2)
#define vmop_ieqC(i0, i1, i2)                 N (0) = N(1) == NC(2)
#define vmop_ineC(i0, i1, i2)                 N (0) = N(1) != NC(2)
#define vmop_igtC(i0, i1, i2)                 N (0) = N(1) >  NC(2)
#define vmop_iltC(i0, i1, i2)                 N (0) = N(1) <  NC(2)
#define vmop_igeC(i0, i1, i2)                 N (0) = N(1) >= NC(2)
#define vmop_ileC(i0, i1, i2)                 N (0) = N(1) <= NC(2)
#define vmop_JieqC(i0, i1, i2)                if (!(N(1) == NC(2))) __JMP__()
#define vmop_JineC(i0, i1, i2)                if (!(N(1) != NC(2))) __JMP__()
#define vmop_JigtC(i0, i1, i2)                if (!(N(1) >  NC(2))) __JMP__()
#define vmop_JiltC(i0, i1, i2)                if (!(N(1) <  NC(2))) __JMP__()
#define vmop_JigeC(i0, i1, i2)                if (!(N(1) >= NC(2))) __JMP__()
#define vmop_JileC(i0, i1, i2)                if (!(N(1) <= NC(2))) __JMP__()

#define vmop_faddC(i0, i1, i2)                F (0) = F(1) + FC(2)
#define vmop_fsubC(i0, i1, i2)                F (0) = F(1) - FC(2)
#define vmop_fmulC(i0, i1, i2)                F (0) = F(1) * FC(2)
#define vmop_fdivC(i0, i1, i2)                F (0) = F(1) / FC(2)
#define vmop_feqC(i0, i1, i2)                 F (0) = F(1) == FC(2)
#define vmop_fneC(i0, i1, i2)                 F (0) = F(1) != FC(2)
#define vmop_fgtC(i0, i1, i2)                 F (0) = F(1) >  FC(2)
#define vmop_fltC(i0, i1, i2)                 F (0) = F(1) <  FC(2)
#define vmop_fgeC(i0, i1, i2)                 F (0) = F(1) >= FC(2)
#define vmop_fleC(i0, i1, i2)                 F (0) = F(1) <= FC(2)
#define vmop_JfeqC(i0, i1, i2)                if (!(F(1) == FC(2))) __JMP__();
#define vmop_JfneC(i0, i1, i2)                if (!(F(1) != FC(2))) __JMP__();
#define vmop_JfgtC(i0, i1, i2)                if (!(F(1) >  FC(2))) __JMP__();
#define vmop_JfltC(i0, i1, i2)                if (!(F(1) <  FC(2))) __JMP__();
#define vmop_JfgeC(i0, i1, i2)                if (!(F(1) >= FC(2))) __JMP__();
#define vmop_JfleC(i0, i1, i2)                if (!(F(1) <= FC(2))) __JMP__();

