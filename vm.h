#ifndef VM_H
#define VM_H

struct vm;
struct vm_code;
static struct vm *vm_new(void);
static void vm_delete(struct vm *vm);
static void vm_exec(struct vm *vm, struct vm_code *pc);
static struct vm_code *vm_code_init(struct vm *vm, struct vm_code *code);

struct vmcode_builder;
struct vmcode_builder *new_vmcode_builder(struct vm *vm);
struct vmcode_builder *vmcode_builder_init(struct vmcode_builder *cb);
void vmcode_builder_delete(struct vmcode_builder *cb);
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
struct vm_code;
typedef struct vm_code *knh_code_t;
DEF_ARRAY_S_T(code);
struct vmcode_builder {
    struct vm *vm;
    Array(code) *codebuf;
    int codesize;
    struct vm_code *(*emit_code)(struct vmcode_builder *);
    void (*optimize_code)(struct vmcode_builder *, struct knh_Method_t *mtd);
    void (*local_start)(struct vmcode_builder *, int idx);
    void (*local_end)(struct vmcode_builder *, int idx);
    void (*movlr)(struct vmcode_builder *, int idx, Reg_t);
    void (*movrl)(struct vmcode_builder *, Reg_t, int idx);
    void (*nset_f)(struct vmcode_builder *, Reg_t, knh_float_t);
    void (*nset_i)(struct vmcode_builder *, Reg_t, knh_data_t);
    void (*nset)(struct vmcode_builder *, Reg_t, knh_value_t);
    void (*oset)(struct vmcode_builder *cb, Reg_t r, struct knh_Object_t *o);
    void (*nmov)(struct vmcode_builder *, Reg_t, Reg_t);
    void (*omov)(struct vmcode_builder *, Reg_t, Reg_t);
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
#define VM_MAX_REG_SIZE 16
#define VM_REG_SIZE     8
struct vm {
    union {
        knh_value_t regs_[VM_MAX_REG_SIZE];
        struct {
            knh_value_t reg[8];
            knh_value_t ret;
            knh_value_t arg[VM_MAX_REG_SIZE - 8 - 1];
        } r;
    };
    knh_value_t *sp;
};


#endif /* end of include guard: VM_H */

