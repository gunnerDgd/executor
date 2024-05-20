#include "io.h"

#include "../root.h"
#include "../cpu.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>

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
    vp_io_out
        (vp_cpu* self, any_t data)                  {
            if (trait_of(self) != vp_cpu_t) return 0;
            struct kvm_run *run = self->run;
            u8_t           *buf = self->run;
            u64_t           len = 0;

            if (run->io.direction != KVM_EXIT_IO_OUT) return 0;
            if (run->exit_reason  != KVM_EXIT_IO)     return 0;
            if (self->stat != vp_cpu_exit) return 0;
            if (self->thd  != this_thd())  return 0;

            buf += run->io.data_offset;
            len  = run->io.size       ;

            mem_copy(data, buf, len);
            return len;
}

u64_t
    vp_io_in
        (vp_cpu* self, any_t data)                  {
            if (trait_of(self) != vp_cpu_t) return 0;
            struct kvm_run *run = self->run;
            u8_t           *buf = self->run;
            u64_t           len = 0;

            if (run->io.direction != KVM_EXIT_IO_IN) return 0;
            if (run->exit_reason  != KVM_EXIT_IO)    return 0;
            if (self->stat != vp_cpu_exit) return 0;
            if (self->thd  != this_thd())  return 0;

            buf += run->io.data_offset;
            len  = run->io.size       ;

            mem_copy(buf, data, len);
            return len;
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
        (vp_cpu* self)                            {
            if (trait_of(self) != vp_cpu_t) return;
            struct kvm_run *run = self->run;

            if (run->exit_reason != KVM_EXIT_IO) return;
            if (self->root->thd  != this_thd ()) return;
            if (self->stat       != vp_cpu_exit) return;
            self->stat = vp_cpu_ready;
}