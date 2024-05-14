#include "cpu.h"

#include "../kvm.h"
#include "root.h"
#include "pa.h"
#include "va.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

static bool_t
    vp_cpu_do_pio_in
        (vp_bus* bus, u64_t port, any_t buf, u64_t len)  {
            if (trait_of(bus) != vp_bus_t) return false_t;
            if (port >= 64 KB)             return false_t;
            if (!buf)                      return false_t;

            switch (len)                                   {
                case  1: return vp_bus_in8 (bus, port, buf);
                case  2: return vp_bus_in16(bus, port, buf);
                case  4: return vp_bus_in32(bus, port, buf);
                default: return false_t;
            }
}

static bool_t
    vp_cpu_do_pio_out
        (vp_bus* bus, u64_t port, any_t buf, u64_t len)  {
            if (trait_of(bus) != vp_bus_t) return false_t;
            if (port >= 64 KB)             return false_t;
            if (!buf)                      return false_t;

            switch (len)                                    {
                case  1: return vp_bus_out8 (bus, port, buf);
                case  2: return vp_bus_out16(bus, port, buf);
                case  4: return vp_bus_out32(bus, port, buf);
                default: return false_t;
            }
}

static bool_t
    vp_cpu_do_pio
        (vp_cpu* self)                                    {
            if (trait_of(self) != vp_cpu_t) return false_t;
            bool_t dir = self->run->io.direction;
            u64_t  pio = self->run->io.port     ;
            u64_t  len = self->run->io.size     ;
            u8_t*  buf = self->run              ;

            vp_pa  *pa   = vp_root_pa(self->root, pio, 1);
            vp_bus *bus  = vp_pa_sub (pa);

            buf += self->run->io.data_offset;
            if (dir == KVM_EXIT_IO_OUT) return vp_cpu_do_pio_out (bus, pio, buf, len);
            if (dir == KVM_EXIT_IO_IN)  return vp_cpu_do_pio_in  (bus, pio, buf, len);
            return false_t;
}

static bool_t
    vp_cpu_do_mmio_in
        (vp_bus* bus, u64_t addr, any_t buf, u64_t len)  {
            if (trait_of(bus) != vp_bus_t) return false_t;
            if (addr < 64 KB)              return false_t;
            if (!buf)                      return false_t;

            switch (len)                                   {
                case  1: return vp_bus_in8 (bus, addr, buf);
                case  2: return vp_bus_in16(bus, addr, buf);
                case  4: return vp_bus_in32(bus, addr, buf);
                case  8: return vp_bus_in64(bus, addr, buf);
                default: return false_t;
            }
}

static bool_t
    vp_cpu_do_mmio_out
        (vp_bus* bus, u64_t addr, any_t buf, u64_t len)  {
            if (trait_of(bus) != vp_bus_t) return false_t;
            if (addr < 64 KB)              return false_t;
            if (!buf)                      return false_t;

            switch (len)                                    {
                case  1: return vp_bus_out8 (bus, addr, buf);
                case  2: return vp_bus_out16(bus, addr, buf);
                case  4: return vp_bus_out32(bus, addr, buf);
                case  8: return vp_bus_out64(bus, addr, buf);
                default: return false_t;
            }
}

static bool_t
    vp_cpu_do_mmio
        (vp_cpu* self)                                    {
            if (trait_of(self) != vp_cpu_t) return false_t;
            u64_t  addr = self->run->mmio.phys_addr;
            bool_t dir  = self->run->mmio.is_write ;
            u8_t*  buf  = self->run->mmio.data     ;
            u64_t  len  = self->run->mmio.len      ;

            vp_pa  *pa  = vp_root_pa(self->root, addr, len);
            vp_bus *bus = vp_pa_sub (pa);

            if (dir == 1) return vp_cpu_do_mmio_out (bus, addr, buf, len);
            if (dir == 0) return vp_cpu_do_mmio_in  (bus, addr, buf, len);
            return false_t;
}

static bool_t
    vp_cpu_do_run
        (vp_cpu *self)                                    {
            if (trait_of(self) != vp_cpu_t) return false_t;
            struct kvm_run *run = self->run;

            if (ioctl(self->cpu, KVM_RUN, null_t)) return false_t;
            switch (run->exit_reason)                            {
                case KVM_EXIT_MMIO: return vp_cpu_do_mmio(self);
                case KVM_EXIT_IO  : return vp_cpu_do_pio (self);
                default           : return false_t;
            }
}

static u64_t
    vp_cpu_do
        (vp_cpu* self)                                    {
            if (trait_of(self) != vp_cpu_t) return false_t;
            static u64_t ret[64]                         = {
                [KVM_EXIT_UNKNOWN]        = vp_cpu_err     ,
                [KVM_EXIT_EXCEPTION]      = vp_cpu_err     ,
                [KVM_EXIT_INTERNAL_ERROR] = vp_cpu_err     ,
                [KVM_EXIT_HLT]            = vp_cpu_halt    ,
                [KVM_EXIT_SHUTDOWN]       = vp_cpu_shutdown,
                [KVM_EXIT_IO]             = vp_cpu_io      ,
                [KVM_EXIT_MMIO]           = vp_cpu_mmio
            };

            while (vp_cpu_do_run(self));
            u64_t  res = self->run->exit_reason; self->stat = vp_cpu_exit;
            return ret[res];
}

static ord_t
    vp_cpu_do_ord
        (vp_cpu *self, vp_cpu *arg)                       {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg->cpu) return ord_lt;
            if (self->cpu > arg->cpu) return ord_gt;
            return ord_eq;
}

static ord_t
    vp_cpu_do_ord_arg
        (vp_cpu *self, u64_t arg)                         {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg) return ord_lt;
            if (self->cpu > arg) return ord_gt;
            return ord_eq;
}

ops_cmp vp_cpu_ops_cmp = make_ops_cmp (
    vp_cpu_do_ord    ,
    vp_cpu_do_ord_arg
);

obj_ops vp_cpu_ops      = {
    .cmp = &vp_cpu_ops_cmp
};


obj_trait vp_cpu_trait = make_trait (
    vp_cpu_new    ,
    vp_cpu_clone  ,
    null_t        ,
    vp_cpu_del    ,
    sizeof(vp_cpu),
    &vp_cpu_ops
);

obj_trait *vp_cpu_t = &vp_cpu_trait;

bool_t
    vp_cpu_new
        (vp_cpu* self, u32_t count, va_list arg)                            {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            u64_t    uid  = 0ull  ; if (count > 1) uid  = va_arg(arg, u64_t);
            u64_t    cpu;
            u64_t    len;
            any_t    run;

            if (trait_of(root) != vp_root_t) return false_t;
            if (root->thd != this_thd())     return false_t;
            self->stat = vp_cpu_ready;
            self->root = root;
            self->uid  = uid ;

            cpu = ioctl(root->root, KVM_CREATE_VCPU, uid)      ;
            len = ioctl(vp_core.kvm, KVM_GET_VCPU_MMAP_SIZE, 0);
            run = mmap                                         (
                null_t                ,
                len                   ,
                PROT_READ | PROT_WRITE,
                MAP_SHARED            ,
                cpu                   ,
                0
            );

            if (run == -1) return false_t;
            if (len == -1) return false_t;
            if (cpu == -1) return false_t;
            self->run = run;
            self->len = len;
            self->cpu = cpu;

            if (!map_move (&root->cpu, self)) {
                munmap (self->run, self->len);
                return false_t;
            }
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
            close (self->cpu)           ;
}

#define vp_desc_sync(desc, arg)\
    desc.base  = arg.base;     \
    desc.limit = arg.limit;    \

#define vp_seg_sync(seg, arg)   \
    seg.base     = arg.base;    \
    seg.limit    = arg.limit;   \
    seg.selector = arg.selector;\
    seg.type     = arg.type;    \
    seg.present  = arg.present; \
    seg.dpl      = arg.dpl;     \
    seg.db       = arg.db;      \
    seg.s        = arg.s;       \
    seg.l        = arg.l;       \
    seg.g        = arg.g;       \
    seg.avl      = arg.avl;     \

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
        (vp_cpu* self, vp_reg* arg)                          {
            if (trait_of (self) != vp_cpu_t)   return false_t;
            if (self->root->thd != this_thd()) return false_t;
            if (!arg)                          return false_t;
            struct kvm_sregs sregs;
            struct kvm_regs  regs ;

            if (ioctl(self->cpu, KVM_GET_SREGS, &sregs)) return false_t;
            if (ioctl(self->cpu, KVM_GET_REGS , &regs))  return false_t;
            vp_desc_sync(arg->gdt, sregs.gdt);
            vp_desc_sync(arg->idt, sregs.idt);
            vp_seg_sync (arg->cs, sregs.cs);
            vp_seg_sync (arg->ds, sregs.ds);
            vp_seg_sync (arg->es, sregs.es);
            vp_seg_sync (arg->ss, sregs.ss);
            vp_seg_sync (arg->gs, sregs.gs);
            vp_seg_sync (arg->fs, sregs.fs);
            vp_gen_sync (arg->gen, regs);
            arg->cr0 = sregs.cr0;
            arg->cr2 = sregs.cr2;
            arg->cr3 = sregs.cr3;
            arg->cr4 = sregs.cr4;
            arg->cr8 = sregs.cr8;

            return true_t;
}

bool_t
    vp_cpu_sync_in
        (vp_cpu* self, vp_reg* arg)                          {
            if (trait_of (self) != vp_cpu_t)   return false_t;
            if (self->root->thd != this_thd()) return false_t;
            if (!arg)                          return false_t;
            struct kvm_sregs sregs;
            struct kvm_regs  regs ;

            vp_desc_sync(sregs.gdt, arg->gdt);
            vp_desc_sync(sregs.idt, arg->idt);
            vp_seg_sync (sregs.cs, arg->cs);
            vp_seg_sync (sregs.ds, arg->ds);
            vp_seg_sync (sregs.es, arg->es);
            vp_seg_sync (sregs.ss, arg->ss);
            vp_seg_sync (sregs.gs, arg->gs);
            vp_seg_sync (sregs.fs, arg->fs);
            vp_gen_sync (regs, arg->gen);
            sregs.cr0 = arg->cr0;
            sregs.cr2 = arg->cr2;
            sregs.cr3 = arg->cr3;
            sregs.cr4 = arg->cr4;
            sregs.cr8 = arg->cr8;

            if (ioctl(self->cpu, KVM_SET_SREGS, &sregs)) return false_t;
            if (ioctl(self->cpu, KVM_SET_REGS , &regs))  return false_t;
            return true_t;
}

fut*
    vp_cpu_run
        (vp_cpu* self)                                       {
            if (trait_of (self) != vp_cpu_t)   return false_t;
            if (self->root->thd != this_thd()) return false_t;
            if (self->stat != vp_cpu_ready)    return false_t;
            self->stat = vp_cpu_busy;

            thd *run = make (thd) from (2, vp_cpu_do, self);
            fut *ret = thd_fut(run);

            if    (trait_of(ret) != fut_t) goto run_err;
            del   (run);
            return ret;
    run_err:
            self->stat = vp_cpu_ready;
            return false_t;
}

u64_t
    vp_cpu_error
        (vp_cpu* self)                                    {
            if (trait_of(self) != vp_cpu_t) return false_t;
            struct kvm_run *run = self->run;

            switch (run->exit_reason)                                            {
                case KVM_EXIT_INTERNAL_ERROR: return run->internal.suberror      ;
                case KVM_EXIT_UNKNOWN       : return run->hw.hardware_exit_reason;
                default                     : return 0;
            }
}

u64_t
    vp_io_port
        (vp_cpu* self)                               {
            if (trait_of(self) != vp_cpu_t) return -1;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_IO) return -1;
            if (self->root->thd  != this_thd())  return -1;
            if (self->stat       != vp_cpu_exit) return -1;
            return run->io.port;
}

u64_t
    vp_io_len
        (vp_cpu* self)                                 {
            if (trait_of(self) != vp_cpu_t) return 0ull;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_IO) return 0ull;
            if (self->root->thd  != this_thd ()) return 0ull;
            if (self->stat       != vp_cpu_exit) return 0ull;
            return run->io.size;
}

u64_t
    vp_io_dir
        (vp_cpu* self)                               {
            if (trait_of(self) != vp_cpu_t) return -1;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_IO) return -1;
            if (self->root->thd  != this_thd ()) return -1;
            if (self->stat       != vp_cpu_exit) return -1;
            return run->io.direction;
}

void
    vp_io_ready
        (vp_cpu* self, any_t arg)                 {
            if (trait_of(self) != vp_cpu_t) return;
            if (!arg)                       return;
            struct kvm_run *run = self->run;
            u8_t           *buf = run;

            if (run->exit_reason != KVM_EXIT_IO) return;
            if (self->root->thd  != this_thd ()) return;
            if (self->stat       != vp_cpu_exit) return;

            mem_copy(buf + run->io.data_offset, arg, run->io.size);
            self->stat = vp_cpu_ready;
}

u64_t
    vp_mmio_addr
        (vp_cpu* self)                               {
            if (trait_of(self) != vp_cpu_t) return -1;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_MMIO) return -1;
            if (self->root->thd  != this_thd ())   return -1;
            if (self->stat       != vp_cpu_exit)   return -1;
            return run->mmio.phys_addr;
}

u64_t
    vp_mmio_len
        (vp_cpu* self)                              {
            if (trait_of(self) != vp_cpu_t) return 0;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_MMIO) return 0ull;
            if (self->root->thd  != this_thd ())   return 0ull;
            if (self->stat       != vp_cpu_exit)   return 0ull;
            return run->mmio.len;
}

u64_t
    vp_mmio_dir
        (vp_cpu* self)                               {
            if (trait_of(self) != vp_cpu_t) return -1;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_MMIO) return -1;
            if (self->root->thd  != this_thd ())   return -1;
            if (self->stat       != vp_cpu_exit)   return -1;
            return run->mmio.is_write;
}

void
    vp_mmio_ready
        (vp_cpu* self, any_t buf)                 {
            if (trait_of(self) != vp_cpu_t) return;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_MMIO) return;
            if (self->root->thd  != this_thd ())   return;
            if (self->stat       != vp_cpu_exit)   return;

            mem_copy(self->run->mmio.data, buf, self->run->mmio.len);
            self->stat = vp_cpu_ready;
}