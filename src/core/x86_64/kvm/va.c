#include "va.h"

#include "cpu.h"
#include "pa.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>

obj_trait vp_va_trait = make_trait (
    vp_va_new    ,
    vp_va_clone  ,
    null_t       ,
    vp_va_del    ,
    sizeof(vp_va),
    null_t
);

obj_trait *vp_va_t = &vp_va_trait;

bool_t
    vp_va_new
        (vp_va* self, u32_t count, va_list arg)                                 {
            struct vp_cpu *cpu = null_t; if (count > 0) cpu = va_arg(arg, any_t);
            struct vp_pa  *pa  = null_t; if (count > 1) pa  = va_arg(arg, any_t);

            struct kvm_translation va = { .linear_address = pa->pa };
            if (trait_of(cpu) != vp_cpu_t) return false_t;
            if (trait_of(pa)  != vp_pa_t)  return false_t;
            self->len = pa->len;
            self->cpu = cpu;
            self->pa  = pa ;

            if (ioctl(cpu->cpu, KVM_TRANSLATE, &va) < 0) return false_t;
            self->va = va.physical_address;
            return true_t;
}

bool_t vp_va_clone (vp_va* self, vp_va* clone) { return false_t; }
void   vp_va_del   (vp_va* self)               {                 }

reg_t
    vp_va_begin
        (vp_va* self)                               {
            if (trait_of(self) != vp_va_t) return -1;
            return self->va;
}

reg_t
    vp_va_end
        (vp_va* self)                              {
            if (trait_of(self) != vp_va_t) return 0;
            return self->va + self->len;
}

u64_t
    vp_va_len
        (vp_va* self)                              {
            if (trait_of(self) != vp_va_t) return 0;
            return self->len;
}

struct vp_pa*
    vp_va_pa
        (vp_va* self)                                   {
            if (trait_of(self) != vp_va_t) return null_t;
            return self->pa;
}
