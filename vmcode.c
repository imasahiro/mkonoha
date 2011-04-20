static void vm_exec(vm_t *vm, vm_code_t *pc)
{
    register value_t *r =   vm->r.reg;
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
        &&L_op_scall,
        &&L_op_fcall,
        &&L_op_ncall_v,
        &&L_op_ncall_i,
        &&L_op_ncall_p,
        &&L_op_ncall_f,
        &&L_op_ret,
        &&L_op_jmp,
        &&L_op_cast,
        &&L_op_ogetidx,
        &&L_op_osetidx,
        &&L_op_ngetidx,
        &&L_op_nsetidx,
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
    DISPATCH(pc);
    L(halt        ) {_arg0(vmop_halt        );}
    L(exit        ) {_arg0(vmop_exit        );}
    L(local_start ) {_arg1(vmop_local_start );}
    L(local_end   ) {_arg1(vmop_local_end   );}
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
    L(scall       ) {_arg3(vmop_scall       );}
    L(fcall       ) {_arg3(vmop_fcall       );}
    L(ncall_v     ) {_arg3(vmop_ncall_v     );}
    L(ncall_i     ) {_arg3(vmop_ncall_i     );}
    L(ncall_p     ) {_arg3(vmop_ncall_p     );}
    L(ncall_f     ) {_arg3(vmop_ncall_f     );}
    L(ret         ) {_arg1(vmop_ret         );}
    L(jmp         ) {_arg1(vmop_jmp         );}
    L(cast        ) {_arg2(vmop_cast        );}
    L(ogetidx     ) {_arg3(vmop_ogetidx     );}
    L(osetidx     ) {_arg3(vmop_osetidx     );}
    L(ngetidx     ) {_arg3(vmop_ngetidx     );}
    L(nsetidx     ) {_arg3(vmop_nsetidx     );}
    L(thcode      ) {_arg0(vmop_thcode      );}
    L(unused0     ) {_arg1(vmop_unused0     );}
    L(unused1     ) {_arg1(vmop_unused1     );}
    L(unused2     ) {_arg1(vmop_unused2     );}
    L(unused3     ) {_arg1(vmop_unused3     );}
    L(unused4     ) {_arg1(vmop_unused4     );}
    L(unused5     ) {_arg1(vmop_unused5     );}
    L(unused6     ) {_arg1(vmop_unused6     );}
    L(unused7     ) {_arg1(vmop_unused7     );}
}
