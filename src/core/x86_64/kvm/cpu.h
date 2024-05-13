#ifndef VAPOR_CORE_KVM_CPU_H
#define VAPOR_CORE_KVM_CPU_H

#include <core.h>

struct kvm_run;

struct vp_root;
struct vp_pa;
struct vp_va;

typedef struct vp_desc {
    u64_t base ;
    u64_t limit;
}   vp_desc;

typedef struct vp_seg {
    u64_t base    ,
          limit   ,
          selector,
          type    ,
          present ,
          dpl     ,
          db      ,
          s       ,
          l       ,
          g       ,
          avl     ;
}   vp_seg;

typedef struct vp_gen       {
    reg_t rax, rbx, rcx, rdx,
          rsp, rbp,
          rsi, rdi,
          rflags,
          r8 ,
          r9 ,
          r10,
          r11,
          r12,
          r13,
          r14,
          r15,
          rip;
}   vp_gen;

typedef struct vp_reg {
    vp_desc gdt, idt;
    vp_gen  gen;
    vp_seg  ldt,
            cs,
            ds,
            es,
            ss,
            gs,
            fs;
}   vp_reg;

extern obj_trait *vp_cpu_t;
typedef struct    vp_cpu  {
    obj             head;
    struct vp_root *root;
    struct kvm_run *run;
    u64_t           len;
    u64_t           cpu;
    u64_t           uid;
}   vp_cpu;

bool_t vp_cpu_new     (vp_cpu*, u32_t, va_list);
bool_t vp_cpu_clone   (vp_cpu*, vp_cpu*)       ;
void   vp_cpu_del     (vp_cpu*)                ;

bool_t vp_cpu_sync_out(vp_cpu*, vp_reg*);
bool_t vp_cpu_sync_in (vp_cpu*, vp_reg*);

#endif
