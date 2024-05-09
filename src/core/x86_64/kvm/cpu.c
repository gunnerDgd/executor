#include "cpu.h"

#include "../kvm.h"
#include "root.h"
#include "pa.h"
#include "va.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

ord_t
    vp_cpu_do_ord
        (vp_cpu *self, vp_cpu *arg)                       {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg->cpu) return ord_lt;
            if (self->cpu > arg->cpu) return ord_gt;
            return ord_eq;
}

ord_t
    vp_cpu_do_ord_arg
        (vp_cpu *self, u64_t arg)                         {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg) return ord_lt;
            if (self->cpu > arg) return ord_gt;
            return ord_eq;
}

ops_cmp vp_cpu_do_cmp = make_ops_cmp (
    vp_cpu_do_ord_arg,
    vp_cpu_do_ord
);

obj_ops vp_cpu_do      = {
    .cmp = &vp_cpu_do_cmp
};


obj_trait vp_cpu_trait = make_trait (
    vp_cpu_new    ,
    vp_cpu_clone  ,
    null_t        ,
    vp_cpu_del    ,
    sizeof(vp_cpu),
    &vp_cpu_do
);

obj_trait *vp_cpu_t = &vp_cpu_trait;

bool_t
    vp_cpu_new
        (vp_cpu* self, u32_t count, va_list arg)                            {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            u64_t    uid  = 0ull  ; if (count > 1) uid  = va_arg(arg, u64_t);
            if (trait_of(root) != vp_root_t) return false_t;
            self->root = root;
            self->uid  = uid ;

            if ((self->cpu = ioctl(root->root, KVM_CREATE_VCPU, uid)) < 0) return false_t;
            if ((self->len = ioctl(self->cpu, KVM_GET_VCPU_MMAP_SIZE, 0))) return false_t;
            self->run = mmap                                                             (
                null_t                ,
                self->len             ,
                PROT_READ | PROT_WRITE,
                MAP_ANONYMOUS         ,
                self->cpu             ,
                0
            );

            if (!self->run)     return false_t;
            if (!map_push (&root->cpu, self)) {
                munmap (self->run, self->len);
                return false_t;
            }

            del    (self);
            return true_t;
}

bool_t
    vp_cpu_clone
        (vp_cpu* self, vp_cpu* clone) {
            return false_t;
}

void
    vp_cpu_del
        (vp_cpu* self)                  {
            munmap(self->run, self->len);
}

#define vp_desc_sync(desc, arg)\
    desc.base  = arg.base;     \
    desc.limit = arg.limit;    \

#define vp_seg_sync(seg, arg)  \
    seg.base     = arg.base;   \
    seg.limit    = arg.limit;  \
    seg.selector = arg.limit;  \
    seg.type     = arg.type;   \
    seg.present  = arg.present;\
    seg.dpl      = arg.dpl;    \
    seg.db       = arg.db;     \
    seg.s        = arg.s;      \
    seg.l        = arg.l;      \
    seg.g        = arg.g;      \
    seg.avl      = arg.avl;    \

#define vp_gen_sync(gen, arg)\
    gen.rflags = arg.rflags; \
    gen.rax    = arg.rax;    \
    gen.rbx    = arg.rbx;    \
    gen.rcx    = arg.rcx;    \
    gen.rdx    = arg.rdx;    \
    gen.r8     = arg.r8 ;    \
    gen.r9     = arg.r9 ;    \
    gen.r10    = arg.r10;    \
    gen.r11    = arg.r11;    \
    gen.r12    = arg.r12;    \
    gen.r13    = arg.r13;    \
    gen.r14    = arg.r14;    \
    gen.r15    = arg.r15;    \
    gen.rsp    = arg.rsp;    \
    gen.rbp    = arg.rbp;    \
    gen.rsi    = arg.rsi;    \
    gen.rdi    = arg.rdi;    \
    gen.rip    = arg.rip;    \

bool_t
    vp_cpu_sync_out
        (vp_cpu* self, vp_reg* arg)                       {
            if (trait_of(self) != vp_cpu_t) return false_t;
            if (!arg)                       return false_t;
            struct kvm_sregs sregs;
            struct kvm_regs  regs ;

            if (ioctl(self->cpu, KVM_GET_SREGS, &sregs)) return false_t;
            if (ioctl(self->cpu, KVM_GET_REGS , &regs))  return false_t;
            vp_desc_sync(sregs.gdt, arg->gdt);
            vp_desc_sync(sregs.idt, arg->idt);
            vp_seg_sync (sregs.cs, arg->cs);
            vp_seg_sync (sregs.ds, arg->ds);
            vp_seg_sync (sregs.es, arg->es);
            vp_seg_sync (sregs.ss, arg->ss);
            vp_seg_sync (sregs.gs, arg->gs);
            vp_seg_sync (sregs.fs, arg->fs);
            vp_gen_sync (regs, arg->gen);
            return true_t;
}

bool_t
    vp_cpu_sync_in
        (vp_cpu* self, vp_reg* arg)                       {
            if (trait_of(self) != vp_cpu_t) return false_t;
            if (!arg)                       return false_t;
            struct kvm_sregs sregs;
            struct kvm_regs  regs ;

            vp_desc_sync(arg->gdt, sregs.gdt);
            vp_desc_sync(arg->idt, sregs.idt);
            vp_seg_sync (arg->cs, sregs.cs);
            vp_seg_sync (arg->ds, sregs.ds);
            vp_seg_sync (arg->es, sregs.es);
            vp_seg_sync (arg->ss, sregs.ss);
            vp_seg_sync (arg->gs, sregs.gs);
            vp_seg_sync (arg->fs, sregs.fs);
            vp_gen_sync (arg->gen, regs);

            if (ioctl(self->cpu, KVM_SET_SREGS, &sregs)) return false_t;
            if (ioctl(self->cpu, KVM_SET_REGS , &regs))  return false_t;
            return true_t;
}

struct vp_va*
    vp_cpu_trans
        (vp_cpu* self, struct vp_pa* pa)                 {
            if (trait_of(self) != vp_cpu_t) return null_t;
            if (trait_of(pa)   != vp_pa_t)  return null_t;
            return make (vp_va) from                     (
                2   ,
                self,
                pa
            );
}