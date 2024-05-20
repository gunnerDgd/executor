#include "io.h"

#include "../root.h"
#include "../cpu.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>

u64_t
    vp_mmio_out
        (vp_cpu* self, any_t data)                  {
            if (trait_of(self) != vp_cpu_t) return 0;
            struct kvm_run *run = self->run;
            u64_t           len = 0;

            if (run->exit_reason   != KVM_EXIT_MMIO) return 0;
            if (run->mmio.is_write != 1)             return 0;
            if (self->thd  != this_thd ()) return 0;
            if (self->stat != vp_cpu_exit) return 0;
            len = run->mmio.len;

            mem_copy(data, run->mmio.data, len);
            return len;
}

u64_t
    vp_mmio_in
        (vp_cpu* self, any_t data)                  {
            if (trait_of(self) != vp_cpu_t) return 0;
            struct kvm_run *run = self->run;
            u64_t           len = 0;

            if (run->exit_reason   != KVM_EXIT_MMIO) return 0;
            if (run->mmio.is_write != 0)             return 0;
            if (self->thd  != this_thd ()) return 0;
            if (self->stat != vp_cpu_exit) return 0;
            len = run->mmio.len;

            mem_copy(run->mmio.data, data, len);
            return len;
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
        (vp_cpu* self)                            {
            if (trait_of(self) != vp_cpu_t) return;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_MMIO) return;
            if (self->root->thd  != this_thd ())   return;
            if (self->stat       != vp_cpu_exit)   return;
            self->stat = vp_cpu_ready;
}