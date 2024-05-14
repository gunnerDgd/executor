#ifndef VAPOR_CORE_CPU_H
#define VAPOR_CORE_CPU_H

#include <core.h>

#ifdef PRESET_ARCH_X86_64
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

typedef struct vp_reg              {
    u64_t   cr0, cr2, cr3, cr4, cr8;
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
#endif

#define vp_cpu_halt     0
#define vp_cpu_shutdown 1
#define vp_cpu_io       2
#define vp_cpu_mmio     3

#define vp_cpu_ready   (-1)
#define vp_cpu_busy    (-2)
#define vp_cpu_exit    (-3)

extern obj_trait *vp_cpu_t;
typedef struct    vp_cpu { u8_t cpu[128]; } vp_cpu;

bool_t vp_cpu_sync_out(vp_cpu*, vp_reg*);
bool_t vp_cpu_sync_in (vp_cpu*, vp_reg*);
fut*   vp_cpu_run     (vp_cpu*)         ;

u64_t  vp_err         (vp_cpu*);

void   vp_io_ready    (vp_cpu*, any_t);
u64_t  vp_io_port     (vp_cpu*);
u64_t  vp_io_len      (vp_cpu*);
u64_t  vp_io_dir      (vp_cpu*);

void   vp_mmio_ready  (vp_cpu*, any_t);
u64_t  vp_mmio_addr   (vp_cpu*);
u64_t  vp_mmio_len    (vp_cpu*);
u64_t  vp_mmio_dir    (vp_cpu*);

#endif
