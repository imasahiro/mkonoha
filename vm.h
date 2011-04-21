#ifndef VM_H
#define VM_H

struct vm;
struct vm_code;
static struct vm *vm_new(void);
static void vm_delete(struct vm *vm);
static void vm_exec(struct vm *vm, struct vm_code *pc);
static struct vm_code *vm_code_init(struct vm *vm, struct vm_code *code);

#endif /* end of include guard: VM_H */

